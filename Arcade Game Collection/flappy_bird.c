#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include "flappy_bird.h"
#include "common.h"

#define X_SIZE 32
#define Y_SIZE 16
#define PIPE_COUNT 3
#define PIPE_SPACING 15
#define INITIAL_SPEED 100
#define MIN_SPEED 40

typedef struct
{
    int x;
    int y;
} PIX;

typedef struct
{
    PIX bird;
    PIX pipes[PIPE_COUNT];
    int frame;
    int score;
    int gameOver;
    int speed;
} FlappyGame;

static void drawGame(FlappyGame *game);
static void updatePipes(FlappyGame *game);
static void hitTest(FlappyGame *game);

void playFlappyBird()
{
    FlappyGame game;
    srand(time(NULL));

    game.bird.x = 10;
    game.bird.y = 10;
    game.frame = 0;
    game.score = 0;
    game.gameOver = 0;
    game.speed = INITIAL_SPEED;

    for (int i = 0; i < PIPE_COUNT; i++)
    {
        game.pipes[i].x = 25 + PIPE_SPACING * i;
        game.pipes[i].y = (rand() % 7) + 5;
    }

    clear();
    attron(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(LINES / 2 - 2, COLS / 2 - 15, "NOT FLAPPY DUCK");
    attroff(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(LINES / 2, COLS / 2 - 15, "Press UP ARROW to jump");
    mvprintw(LINES / 2 + 1, COLS / 2 - 15, "Press Q to quit");
    mvprintw(LINES / 2 + 3, COLS / 2 - 15, "Press any key to start...");
    refresh();
    timeout(-1);
    getch();

    timeout(game.speed);

    while (!game.gameOver)
    {
        int ch = getch();

        if (ch == KEY_UP || ch == ' ')
        {
            game.bird.y -= 2;
            if (game.bird.y < 1)
                game.bird.y = 1;
        }
        else if (ch == 'q' || ch == 'Q')
        {
            break;
        }

        if (game.frame == 2)
        {
            game.bird.y++;

            for (int i = 0; i < PIPE_COUNT; i++)
            {
                game.pipes[i].x--;
                if (game.pipes[i].x == game.bird.x - 3)
                {
                    game.score++;

                    // Increase difficulty every 3 points
                    if (game.score % 3 == 0 && game.speed > MIN_SPEED)
                    {
                        game.speed -= 5;
                        timeout(game.speed);
                    }
                }
            }

            game.frame = 0;
        }

        hitTest(&game);
        drawGame(&game);
        updatePipes(&game);

        game.frame++;
    }

    clear();
    attron(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
    mvprintw(LINES / 2, COLS / 2 - 10, "GAME OVER!");
    attroff(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
    mvprintw(LINES / 2 + 1, COLS / 2 - 10, "Score: %d", game.score);
    mvprintw(LINES / 2 + 2, COLS / 2 - 15, "Press any key to return to menu...");
    refresh();
    timeout(-1);
    getch();
}

static void drawGame(FlappyGame *game)
{
    clear();

    // Draw borders
    for (int y = 0; y <= Y_SIZE; y++)
    {
        for (int x = 0; x <= X_SIZE; x++)
        {
            if (y == 0 || y == Y_SIZE || x == 0 || x == X_SIZE)
            {
                attron(COLOR_PAIR(COLOR_GROUND));
                mvprintw(y, x * 2, "[]");
                attroff(COLOR_PAIR(COLOR_GROUND));
            }
        }
    }

    // Draw pipes
    for (int i = 0; i < PIPE_COUNT; i++)
    {
        int px = game->pipes[i].x;
        int py = game->pipes[i].y;

        if (px < 0 || px > X_SIZE)
            continue;

        attron(COLOR_PAIR(COLOR_PIPE) | A_BOLD);

        for (int y = 1; y < Y_SIZE; y++)
        {
            for (int x = 1; x < X_SIZE; x++)
            {
                if (px >= x - 1 && px <= x + 1 && (py == y + 3 || py == y - 3))
                {
                    mvprintw(y, x * 2, "[]");
                }
                else if (px == x - 1 && py == y - 4)
                {
                    mvprintw(y, x * 2, "]/");
                }
                else if (px == x && (py <= y - 4 || py >= y + 4))
                {
                    mvprintw(y, x * 2, "][");
                }
                else if (px == x + 1 && py == y - 4)
                {
                    mvprintw(y, x * 2, "\\[");
                }
                else if (px == x - 1 && py == y + 4)
                {
                    mvprintw(y, x * 2, "]\\");
                }
                else if (px == x + 1 && py == y + 4)
                {
                    mvprintw(y, x * 2, "/[");
                }
                else if (px == x + 1 && (py <= y - 5 || py >= y + 5))
                {
                    mvprintw(y, x * 2, " [");
                }
                else if (px == x - 1 && (py <= y - 5 || py >= y + 5))
                {
                    mvprintw(y, x * 2, "] ");
                }
            }
        }

        attroff(COLOR_PAIR(COLOR_PIPE) | A_BOLD);
    }

    int bx = game->bird.x;
    int by = game->bird.y;

    attron(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);

    if (by - 1 >= 0 && by - 1 <= Y_SIZE)
    {
        mvprintw(by - 1, bx * 2, ") ");
        mvprintw(by - 1, (bx + 1) * 2, "__");
        mvprintw(by - 1, (bx + 2) * 2, " \\");
    }

    if (by >= 0 && by <= Y_SIZE)
    {
        mvprintw(by, bx * 2, ")>");
        mvprintw(by, (bx + 1) * 2, "_(");
        mvprintw(by, (bx + 2) * 2, " _");
    }

    attroff(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);

    attron(COLOR_PAIR(COLOR_MENU));
    mvprintw(Y_SIZE + 1, 0, "Score: %d | Speed: %d | Q=quit",
             game->score, INITIAL_SPEED - game->speed);
    attroff(COLOR_PAIR(COLOR_MENU));

    refresh();
}

static void updatePipes(FlappyGame *game)
{
    for (int i = 0; i < PIPE_COUNT; i++)
    {
        if (game->pipes[i].x == -1)
        {
            if (i == 0)
            {
                game->pipes[i].x = game->pipes[2].x + PIPE_SPACING;
            }
            else
            {
                game->pipes[i].x = game->pipes[i - 1].x + PIPE_SPACING;
            }
            game->pipes[i].y = (rand() % 7) + 5;
        }
    }
}

static void hitTest(FlappyGame *game)
{
    if (game->bird.y >= Y_SIZE - 1)
    {
        game->gameOver = 1;
        return;
    }

    for (int i = 0; i < PIPE_COUNT; i++)
    {
        if ((game->bird.x - 2 < game->pipes[i].x + 2) &&
            (game->bird.x > game->pipes[i].x - 2) &&
            ((game->bird.y < game->pipes[i].y - 2) ||
             (game->bird.y > game->pipes[i].y + 1)))
        {
            game->gameOver = 1;
            return;
        }
    }
}
