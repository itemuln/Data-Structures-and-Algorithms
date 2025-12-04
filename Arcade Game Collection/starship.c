#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "temple_run.h"

#define MAX_BULLETS 100
#define FIRERATE 500
#define GAME_SPEED 75

typedef struct
{
    int x;
    int y;
} PIX;

typedef struct
{
    int playerY;
    int score;
    PIX bullets[MAX_BULLETS];
    PIX enemy;
    int bulletCount;
    int fire;
    int gameOver;
    char input;
    pthread_t inputThread;
} SpaceShooterGame;

static SpaceShooterGame *game_ptr = NULL;

void *inputThreadFunc(void *arg)
{
    (void)arg;
    while (!game_ptr->gameOver)
    {
        int ch = getch();
        if (ch != ERR)
        {
            game_ptr->input = (char)ch;
        }
        msleep(10);
    }
    return NULL;
}

static void spawnEnemy(SpaceShooterGame *game)
{
    game->enemy.y = (rand() % 14) + 2;
    game->enemy.x = WIDTH - 6;
}

static void spawnBullet(SpaceShooterGame *game)
{
    if (game->bulletCount >= MAX_BULLETS)
        return;
    game->fire = FIRERATE / GAME_SPEED;
    game->bullets[game->bulletCount].x = 6;
    game->bullets[game->bulletCount].y = game->playerY + 1;
    game->bulletCount++;
}

static void despawnBullet(SpaceShooterGame *game, int idx)
{
    for (int i = idx; i < game->bulletCount - 1; i++)
    {
        game->bullets[i] = game->bullets[i + 1];
    }
    game->bulletCount--;
}

static int inPlayer(SpaceShooterGame *game)
{
    if (game->enemy.x == 4 && game->enemy.y == game->playerY + 1)
        return 1;
    if (game->enemy.x == 3 && (game->enemy.y == game->playerY || game->enemy.y == game->playerY + 2))
        return 1;
    if (game->enemy.x == 1 && (game->enemy.y == game->playerY + 3 || game->enemy.y == game->playerY - 1))
        return 1;
    return 0;
}

static void hitTest(SpaceShooterGame *game)
{
    if (game->enemy.x <= 0)
    {
        game->gameOver = 1;
        return;
    }

    if (inPlayer(game))
    {
        game->gameOver = 1;
        return;
    }

    for (int i = 0; i < game->bulletCount; i++)
    {
        if (game->bullets[i].y >= game->enemy.y - 1 &&
            game->bullets[i].y <= game->enemy.y + 1 &&
            game->bullets[i].x >= game->enemy.x &&
            game->bullets[i].x <= game->enemy.x + 3)
        {
            game->score += 100;
            spawnEnemy(game);
            despawnBullet(game, i);
            break;
        }
    }
}

static void moveElements(SpaceShooterGame *game)
{
    game->enemy.x--;

    for (int i = 0; i < game->bulletCount; i++)
    {
        if (game->bullets[i].x >= WIDTH - 2)
        {
            despawnBullet(game, i);
            i--;
        }
        else
        {
            game->bullets[i].x += 2;
        }
    }

    switch (game->input)
    {
    case 'w':
    case 'W':
        if (game->playerY > 1)
            game->playerY--;
        break;
    case 's':
    case 'S':
        if (game->playerY < HEIGHT - 4)
            game->playerY++;
        break;
    case ' ':
        if (game->fire == 0)
            spawnBullet(game);
        break;
    case 'q':
    case 'Q':
        game->gameOver = 1;
        break;
    }

    game->input = '\0';
    hitTest(game);
}

static void drawGame(SpaceShooterGame *game)
{
    clear();

    attron(COLOR_PAIR(COLOR_MENU));
    mvprintw(0, 0, "Q=quit | W/S=move | SPACE=shoot | Score: %d", game->score);
    attroff(COLOR_PAIR(COLOR_MENU));

    // Draw borders
    for (int i = 1; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (i == 1 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
            {
                attron(COLOR_PAIR(COLOR_GROUND));
                mvaddch(i, j, '#');
                attroff(COLOR_PAIR(COLOR_GROUND));
            }
        }
    }

    // Draw player
    int py = game->playerY;
    attron(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
    mvprintw(py, 2, " /]\\");
    mvprintw(py + 1, 2, "<]()]]C>");
    mvprintw(py + 2, 2, " \\]/");
    attroff(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);

    // Draw bullets
    attron(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
    for (int i = 0; i < game->bulletCount; i++)
    {
        if (game->bullets[i].x > 0 && game->bullets[i].x < WIDTH - 1 &&
            game->bullets[i].y > 1 && game->bullets[i].y < HEIGHT - 1)
        {
            mvprintw(game->bullets[i].y, game->bullets[i].x, "()");
        }
    }
    attroff(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);

    // Draw enemy
    int ex = game->enemy.x;
    int ey = game->enemy.y;
    if (ex > 0 && ex < WIDTH - 5 && ey > 1 && ey < HEIGHT - 2)
    {
        attron(COLOR_PAIR(COLOR_SPIKE) | A_BOLD);
        mvprintw(ey - 1, ex + 1, " /-|");
        mvprintw(ey, ex, " <( [>C>");
        mvprintw(ey + 1, ex + 1, " \\-|");
        attroff(COLOR_PAIR(COLOR_SPIKE) | A_BOLD);
    }

    refresh();
}

void playTempleRun()
{
    SpaceShooterGame game;
    game.playerY = HEIGHT / 2;
    game.score = 0;
    game.bulletCount = 0;
    game.fire = 0;
    game.gameOver = 0;
    game.input = '\0';
    game_ptr = &game;

    srand(time(NULL));
    spawnEnemy(&game);

    clear();
    attron(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(HEIGHT / 2 - 2, WIDTH / 2 - 10, "SPACE SHOOTER");
    attroff(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(HEIGHT / 2, WIDTH / 2 - 15, "W/S to move, SPACE to shoot, Q to quit");
    refresh();
    msleep(2000);

    timeout(0);
    pthread_create(&game.inputThread, NULL, inputThreadFunc, NULL);

    while (!game.gameOver)
    {
        moveElements(&game);
        drawGame(&game);

        if (game.fire > 0)
            game.fire--;

        msleep(GAME_SPEED);
    }

    pthread_cancel(game.inputThread);
    pthread_join(game.inputThread, NULL);
    game_ptr = NULL;

    clear();
    attron(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
    mvprintw(HEIGHT / 2, WIDTH / 2 - 10, "GAME OVER!");
    attroff(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
    mvprintw(HEIGHT / 2 + 1, WIDTH / 2 - 10, "Final Score: %d", game.score);
    refresh();
    timeout(-1);
    getch();
}