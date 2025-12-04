#include <stdlib.h>
#include "dino_run.h"
#include "common.h"

#define GROUND_Y (HEIGHT - 3)
#define DINO_X 5
#define JUMP_HEIGHT -4
#define DUCK_OFFSET 2

typedef struct
{
    int playerY;
    int velocityY;
    int score;
    int isDucking;
    int speed;
    Obstacle *obstacles;
    int gameOver;
} DinoGame;

void playDinoRun()
{
    DinoGame game;
    game.playerY = GROUND_Y;
    game.velocityY = 0;
    game.score = 0;
    game.isDucking = 0;
    game.speed = 150;
    game.obstacles = NULL;
    game.gameOver = 0;

    clear();
    attron(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(0, 0, "DINO RUN - Chrome Dinosaur Game!");
    attroff(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(2, 0, "SPACE = Jump | S = Duck | Q = Quit");
    refresh();
    msleep(2000);

    timeout(game.speed);
    int frameCount = 0;

    while (!game.gameOver)
    {
        // Physics
        if (game.playerY < GROUND_Y)
        {
            game.velocityY++;
            game.playerY += game.velocityY;
        }

        if (game.playerY >= GROUND_Y)
        {
            game.playerY = GROUND_Y;
            game.velocityY = 0;
        }

        // Move obstacles
        Obstacle *current = game.obstacles;
        Obstacle *prev = NULL;

        while (current != NULL)
        {
            current->x -= 2;

            // Collision with cactus
            if (current->symbol == 'X' && current->x >= DINO_X - 2 && current->x <= DINO_X + 2)
            {
                if (game.isDucking && game.playerY == GROUND_Y)
                {
                    // Safe - ducking under
                }
                else if (game.playerY >= current->y - 1)
                {
                    game.gameOver = 1;
                }
            }

            // Collision with bird
            if (current->symbol == 'B' && current->x >= DINO_X - 2 && current->x <= DINO_X + 2)
            {
                if (game.playerY <= current->y + 1 && game.playerY >= current->y - 1)
                {
                    game.gameOver = 1;
                }
            }

            if (current->x < -5)
            {
                game.score += 10;
                if (prev == NULL)
                {
                    game.obstacles = current->next;
                    free(current);
                    current = game.obstacles;
                }
                else
                {
                    prev->next = current->next;
                    free(current);
                    current = prev->next;
                }
            }
            else
            {
                prev = current;
                current = current->next;
            }
        }

        // Spawn obstacles
        frameCount++;
        if (frameCount % 30 == 0)
        {
            int type = rand() % 3;
            Obstacle *obs;

            if (type == 0)
            {
                // Tall cactus
                obs = createObstacle(WIDTH - 2, GROUND_Y, 'X');
            }
            else if (type == 1)
            {
                // Short cactus
                obs = createObstacle(WIDTH - 2, GROUND_Y, 'x');
            }
            else
            {
                // Flying bird
                obs = createObstacle(WIDTH - 2, GROUND_Y - 4, 'B');
            }

            obs->next = game.obstacles;
            game.obstacles = obs;
        }

        // Increase speed
        if (game.score % 100 == 0 && game.speed > 50)
        {
            game.speed -= 10;
            timeout(game.speed);
        }

        // Draw
        clear();

        // Draw ground
        attron(COLOR_PAIR(COLOR_GROUND));
        for (int i = 0; i < WIDTH; i++)
        {
            mvaddch(HEIGHT - 2, i, '=');
            mvaddch(HEIGHT - 1, i, '#');
        }
        attroff(COLOR_PAIR(COLOR_GROUND));

        // Draw obstacles
        current = game.obstacles;
        while (current != NULL)
        {
            if (current->x >= 0 && current->x < WIDTH)
            {
                if (current->symbol == 'X')
                {
                    // Tall cactus
                    attron(COLOR_PAIR(COLOR_SPIKE) | A_BOLD);
                    mvprintw(current->y, current->x, "||");
                    mvprintw(current->y - 1, current->x, "||");
                    mvprintw(current->y - 2, current->x, "||");
                    attroff(COLOR_PAIR(COLOR_SPIKE) | A_BOLD);
                }
                else if (current->symbol == 'x')
                {
                    // Short cactus
                    attron(COLOR_PAIR(COLOR_SPIKE) | A_BOLD);
                    mvprintw(current->y, current->x, "||");
                    attroff(COLOR_PAIR(COLOR_SPIKE) | A_BOLD);
                }
                else if (current->symbol == 'B')
                {
                    // Bird
                    attron(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
                    mvprintw(current->y, current->x, "<>");
                    attroff(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
                }
            }
            current = current->next;
        }

        // Draw dino
        attron(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
        if (game.isDucking && game.playerY == GROUND_Y)
        {
            mvprintw(game.playerY, DINO_X, "_/\\_");
        }
        else
        {
            mvprintw(game.playerY - 2, DINO_X, " __");
            mvprintw(game.playerY - 1, DINO_X, "/ o\\");
            mvprintw(game.playerY, DINO_X, "\\__/");
        }
        attroff(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);

        // Draw HUD
        attron(COLOR_PAIR(COLOR_MENU));
        mvprintw(0, 0, "Score: %d | Speed: %d", game.score, 150 - game.speed);
        attroff(COLOR_PAIR(COLOR_MENU));

        refresh();

        // Input
        int ch = getch();
        game.isDucking = 0;

        if (ch == ' ' && game.playerY == GROUND_Y)
        {
            game.velocityY = JUMP_HEIGHT;
        }
        else if (ch == 's' || ch == 'S')
        {
            game.isDucking = 1;
        }
        else if (ch == 'q' || ch == 'Q')
        {
            break;
        }
    }

    freeObstacles(game.obstacles);
    clear();
    attron(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
    mvprintw(HEIGHT / 2, WIDTH / 2 - 10, "Game Over! Score: %d", game.score);
    attroff(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
    mvprintw(HEIGHT / 2 + 1, WIDTH / 2 - 15, "Press any key to return...");
    refresh();
    timeout(-1);
    getch();
}
