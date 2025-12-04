#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "geometry_dash.h"
#include "common.h"

int ballX = 25;
int ballY = 10;
int vectorBallX = 1;
int vectorBallY = 0;

int leftPosY = 10;
int rightPosY = 10;
const int leftPosX = 5;
const int rightPosX = 45;

int pointL = 0;
int pointR = 0;
int winCheck = 0;
char lastButton = ' ';
int winScore = 5;    // Add variable for match length
int gameSpeed = 100; // Add variable for difficulty

void drawPong(int pointLeft, int pointRight, int leftPositionY, int rightPositionY,
              int ballPositionX, int ballPositionY);
void moveBall();
void clash();
void checkWinner(int left, int right);

void playGeometryDash()
{
    ballX = 25;
    ballY = 10;
    vectorBallX = 1;
    vectorBallY = 0;
    leftPosY = 10;
    rightPosY = 10;
    pointL = 0;
    pointR = 0;
    winCheck = 0;
    lastButton = ' ';

    // Choose match length
    clear();
    attron(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(HEIGHT / 2 - 2, WIDTH / 2 - 10, "=== PONG ===");
    attroff(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(HEIGHT / 2, WIDTH / 2 - 10, "Select Match Length:");
    mvprintw(HEIGHT / 2 + 1, WIDTH / 2 - 10, "1. Best of 3 (2 pts)");
    mvprintw(HEIGHT / 2 + 2, WIDTH / 2 - 10, "2. Best of 5 (3 pts)");
    mvprintw(HEIGHT / 2 + 3, WIDTH / 2 - 10, "3. Best of 7 (4 pts)");
    refresh();

    int choice = getch();
    if (choice == '1')
        winScore = 2;
    else if (choice == '2')
        winScore = 3;
    else if (choice == '3')
        winScore = 4;
    else
        winScore = 3;

    // Choose difficulty
    clear();
    attron(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(HEIGHT / 2 - 1, WIDTH / 2 - 10, "Select Difficulty:");
    attroff(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(HEIGHT / 2, WIDTH / 2 - 10, "1. Easy (Slow)");
    mvprintw(HEIGHT / 2 + 1, WIDTH / 2 - 10, "2. Normal");
    mvprintw(HEIGHT / 2 + 2, WIDTH / 2 - 10, "3. Hard (Fast)");
    refresh();

    choice = getch();
    if (choice == '1')
        gameSpeed = 120;
    else if (choice == '2')
        gameSpeed = 100;
    else if (choice == '3')
        gameSpeed = 80;
    else
        gameSpeed = 100;

    clear();
    attron(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(HEIGHT / 2 - 3, WIDTH / 2 - 8, "=== PONG ===");
    attroff(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(HEIGHT / 2 - 1, WIDTH / 2 - 15, "Left: Z=Down A=Up | Right: K=Down M=Up");
    mvprintw(HEIGHT / 2, WIDTH / 2 - 10, "SPACE=Skip | Q=Quit");
    mvprintw(HEIGHT / 2 + 1, WIDTH / 2 - 12, "First to %d points wins!", winScore);
    mvprintw(HEIGHT / 2 + 3, WIDTH / 2 - 10, "Press any key to start");
    refresh();
    timeout(-1);
    getch();

    timeout(gameSpeed); // Use selected speed
    drawPong(pointL, pointR, leftPosY, rightPosY, ballX, ballY);

    while (winCheck == 0)
    {
        int ch = getch();

        switch (ch)
        {
        case 'z':
        case 'Z':
            lastButton = 'z';
            leftPosY++;
            moveBall();
            clash();
            drawPong(pointL, pointR, leftPosY, rightPosY, ballX, ballY);
            break;
        case 'a':
        case 'A':
            lastButton = 'a';
            leftPosY--;
            moveBall();
            clash();
            drawPong(pointL, pointR, leftPosY, rightPosY, ballX, ballY);
            break;
        case 'm':
        case 'M':
            lastButton = 'm';
            rightPosY++;
            moveBall();
            clash();
            drawPong(pointL, pointR, leftPosY, rightPosY, ballX, ballY);
            break;
        case 'k':
        case 'K':
            lastButton = 'k';
            rightPosY--;
            moveBall();
            clash();
            drawPong(pointL, pointR, leftPosY, rightPosY, ballX, ballY);
            break;
        case ' ':
            lastButton = ' ';
            moveBall();
            clash();
            drawPong(pointL, pointR, leftPosY, rightPosY, ballX, ballY);
            break;
        case 'q':
        case 'Q':
            winCheck = 3;
            break;
        case ERR:
            moveBall();
            clash();
            drawPong(pointL, pointR, leftPosY, rightPosY, ballX, ballY);
            break;
        }
    }

    clear();
    attron(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(HEIGHT / 2 - 2, WIDTH / 2 - 6, "GAME OVER!");
    attroff(COLOR_PAIR(COLOR_MENU) | A_BOLD);

    if (winCheck == 1)
    {
        attron(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
        mvprintw(HEIGHT / 2, WIDTH / 2 - 7, "Left Player Won!");
        attroff(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
    }
    else if (winCheck == 2)
    {
        attron(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
        mvprintw(HEIGHT / 2, WIDTH / 2 - 8, "Right Player Won!");
        attroff(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
    }

    mvprintw(HEIGHT / 2 + 2, WIDTH / 2 - 8, "Final: %d - %d", pointL, pointR);
    mvprintw(HEIGHT / 2 + 4, WIDTH / 2 - 12, "Press any key to continue");
    refresh();
    timeout(-1);
    getch();
}

void drawPong(int pointLeft, int pointRight, int leftPositionY, int rightPositionY,
              int ballPositionX, int ballPositionY)
{
    clear();

    attron(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
    mvprintw(1, WIDTH / 4, "Left: %d", pointLeft);
    attroff(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);

    attron(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
    mvprintw(1, 3 * WIDTH / 4, "Right: %d", pointRight);
    attroff(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);

    if (winCheck == 1 || winCheck == 2)
        return;

    attron(COLOR_PAIR(COLOR_GROUND));
    for (int i = 0; i < WIDTH; i++)
    {
        mvaddch(0, i, '-');
        mvaddch(HEIGHT - 1, i, '-');
    }
    for (int i = 0; i < HEIGHT; i++)
    {
        mvaddch(i, 0, '|');
        mvaddch(i, WIDTH - 1, '|');
    }
    attroff(COLOR_PAIR(COLOR_GROUND));

    attron(COLOR_PAIR(COLOR_GROUND) | A_DIM);
    for (int i = 1; i < HEIGHT - 1; i++)
    {
        if (ballPositionY != i || ballPositionX != WIDTH / 2)
            mvaddch(i, WIDTH / 2, '|');
    }
    attroff(COLOR_PAIR(COLOR_GROUND) | A_DIM);

    attron(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvaddch(ballPositionY, ballPositionX, '*');
    attroff(COLOR_PAIR(COLOR_MENU) | A_BOLD);

    attron(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
    mvaddch(leftPositionY, leftPosX, '|');
    mvaddch(leftPositionY + 1, leftPosX, '|');
    mvaddch(leftPositionY - 1, leftPosX, '|');
    attroff(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);

    attron(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
    mvaddch(rightPositionY, rightPosX, '|');
    mvaddch(rightPositionY + 1, rightPosX, '|');
    mvaddch(rightPositionY - 1, rightPosX, '|');
    attroff(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);

    refresh();
}

void moveBall()
{
    ballX += vectorBallX;
    ballY += vectorBallY;
}

void clash()
{
    if ((ballY <= 1 && (ballX >= 1 && ballX <= WIDTH - 2)) ||
        (ballY >= HEIGHT - 2 && (ballX >= 1 && ballX <= WIDTH - 2)))
    {
        vectorBallY *= -1;
    }

    if (ballX <= 1 && (ballY >= 1 && ballY <= HEIGHT - 2))
    {
        pointR++;

        clear();
        attron(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
        mvprintw(HEIGHT / 2, WIDTH / 2 - 5, "RIGHT SCORES!");
        attroff(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
        refresh();
        msleep(500);

        for (int i = 3; i > 0; i--)
        {
            clear();
            attron(COLOR_PAIR(COLOR_MENU) | A_BOLD);
            mvprintw(HEIGHT / 2, WIDTH / 2, "%d", i);
            attroff(COLOR_PAIR(COLOR_MENU) | A_BOLD);
            refresh();
            msleep(800);
        }

        ballX = 25;
        ballY = 10;
        leftPosY = 10;
        rightPosY = 10;
        vectorBallX = -1;
        vectorBallY = 0;
        checkWinner(pointL, pointR);
    }

    if (ballX >= WIDTH - 2 && (ballY >= 1 && ballY <= HEIGHT - 2))
    {
        pointL++;

        clear();
        attron(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
        mvprintw(HEIGHT / 2, WIDTH / 2 - 5, "LEFT SCORES!");
        attroff(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
        refresh();
        msleep(500);

        for (int i = 3; i > 0; i--)
        {
            clear();
            attron(COLOR_PAIR(COLOR_MENU) | A_BOLD);
            mvprintw(HEIGHT / 2, WIDTH / 2, "%d", i);
            attroff(COLOR_PAIR(COLOR_MENU) | A_BOLD);
            refresh();
            msleep(800);
        }

        ballX = 25;
        ballY = 10;
        rightPosY = 10;
        leftPosY = 10;
        vectorBallX = 1;
        vectorBallY = 0;
        checkWinner(pointL, pointR);
    }

    if ((ballX == leftPosX + 1) &&
        (ballY == leftPosY || ballY == leftPosY + 1 || ballY == leftPosY - 1))
    {
        vectorBallX *= -1;
        switch (lastButton)
        {
        case 'a':
            vectorBallY = 1;
            break;
        case 'z':
            vectorBallY = -1;
            break;
        case 'k':
            vectorBallY = 1;
            break;
        case 'm':
            vectorBallY = -1;
            break;
        }
    }

    if (ballX == rightPosX - 1 &&
        (ballY == rightPosY || ballY == rightPosY + 1 || ballY == rightPosY - 1))
    {
        vectorBallX *= -1;
        switch (lastButton)
        {
        case 'a':
            vectorBallY = 1;
            break;
        case 'z':
            vectorBallY = -1;
            break;
        case 'k':
            vectorBallY = 1;
            break;
        case 'm':
            vectorBallY = -1;
            break;
        }
    }

    if (leftPosY < 2)
        leftPosY++;
    if (leftPosY > HEIGHT - 3)
        leftPosY--;
    if (rightPosY < 2)
        rightPosY++;
    if (rightPosY > HEIGHT - 3)
        rightPosY--;
}

void checkWinner(int left, int right)
{
    if (left == winScore) 
    {
        winCheck = 1;
    }
    if (right == winScore)
    {
        winCheck = 2;
    }
}