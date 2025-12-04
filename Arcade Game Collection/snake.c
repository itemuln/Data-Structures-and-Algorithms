#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <time.h>
#include <string.h>
#include "snake.h"
#include "common.h"

#define MAX_SCORE 256
#define INITIAL_FRAME_TIME 110000
#define MIN_FRAME_TIME 30000
#define MAX_SNAKE_OBSTACLES 50
#define MAX_ENEMIES 10

typedef struct
{
    int x;
    int y;
} vec2;

int snake_score = 0;
char score_message[16];
int current_frame_time = INITIAL_FRAME_TIME;
int last_speed_increase = 0;

bool skip = false;
bool is_running = true;

int screen_width = 25;
int screen_height = 20;

vec2 snake_head = {0, 0};
vec2 segments[MAX_SCORE + 1];
vec2 snake_dir = {1, 0};
vec2 snake_berry;

typedef struct
{
    vec2 pos;
    bool active;
} SnakeObstacle;

SnakeObstacle obstacles[MAX_SNAKE_OBSTACLES];
int obstacle_count = 0;

// Hooh elementuud
typedef struct
{
    vec2 pos;
    bool active;
    int move_counter;
} Enemy;

Enemy enemies[MAX_ENEMIES];
int enemy_count = 0;
int enemy_spawn_counter = 0;

bool collide(vec2 a, vec2 b)
{
    if (a.x == b.x && a.y == b.y)
    {
        return true;
    }
    else
        return false;
}

bool collide_snake_body(vec2 point)
{
    for (int i = 0; i < snake_score; i++)
    {
        if (collide(point, segments[i]))
        {
            return true;
        }
    }
    return false;
}

bool collide_obstacle(vec2 point)
{
    for (int i = 0; i < obstacle_count; i++)
    {
        if (obstacles[i].active && collide(point, obstacles[i].pos))
        {
            return true;
        }
    }
    return false;
}

bool collide_enemy(vec2 point)
{
    for (int i = 0; i < enemy_count; i++)
    {
        if (enemies[i].active && collide(point, enemies[i].pos))
        {
            return true;
        }
    }
    return false;
}

vec2 spawn_berry()
{
    vec2 berry = {1 + rand() % (screen_width - 2), 1 + rand() % (screen_height - 2)};
    while (collide(snake_head, berry) || collide_snake_body(berry) ||
           collide_obstacle(berry) || collide_enemy(berry))
    {
        berry.x = 1 + rand() % (screen_width - 2);
        berry.y = 1 + rand() % (screen_height - 2);
    }
    return berry;
}

void spawn_obstacle()
{
    if (obstacle_count >= MAX_SNAKE_OBSTACLES)
        return;

    vec2 pos = {1 + rand() % (screen_width - 2), 1 + rand() % (screen_height - 2)};
    // snake esvel enemy deer garahgui baih
    while (collide(snake_head, pos) || collide_snake_body(pos) ||
           collide(snake_berry, pos) || collide_obstacle(pos))
    {
        pos.x = 1 + rand() % (screen_width - 2);
        pos.y = 1 + rand() % (screen_height - 2);
    }

    obstacles[obstacle_count].pos = pos;
    obstacles[obstacle_count].active = true;
    obstacle_count++;
}

void spawn_enemy()
{
    if (enemy_count >= MAX_ENEMIES)
        return;

    // enemey spawn hiih
    vec2 pos;
    int edge = rand() % 4;
    switch (edge)
    {
    case 0:
        pos.x = 0;
        pos.y = rand() % screen_height;
        break;
    case 1:
        pos.x = screen_width - 1;
        pos.y = rand() % screen_height;
        break;
    case 2:
        pos.x = rand() % screen_width;
        pos.y = 0;
        break;
    case 3:
        pos.x = rand() % screen_width;
        pos.y = screen_height - 1;
        break;
    }

    enemies[enemy_count].pos = pos;
    enemies[enemy_count].active = true;
    enemies[enemy_count].move_counter = 0;
    enemy_count++;
}

void update_enemies()
{
    for (int i = 0; i < enemy_count; i++)
    {
        if (!enemies[i].active)
            continue;

        enemies[i].move_counter++;
        // 5 frame bolgond hudluh
        if (enemies[i].move_counter < 5)
            continue;

        enemies[i].move_counter = 0;

        // ai magic
        vec2 new_pos = enemies[i].pos;

        if (enemies[i].pos.x < snake_head.x)
            new_pos.x++;
        else if (enemies[i].pos.x > snake_head.x)
            new_pos.x--;

        if (enemies[i].pos.y < snake_head.y)
            new_pos.y++;
        else if (enemies[i].pos.y > snake_head.y)
            new_pos.y--;

        // hudluh nuhtsul
        if (!collide_obstacle(new_pos) && !collide_snake_body(new_pos))
        {
            enemies[i].pos = new_pos;
        }
    }
}

void draw_border_snake(int y, int x, int width, int height)
{
    attron(COLOR_PAIR(COLOR_GROUND));
    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(y, x + width * 2 + 1, ACS_URCORNER);
    for (int i = 1; i < width * 2 + 1; i++)
    {
        mvaddch(y, x + i, ACS_HLINE);
    }
    for (int i = 1; i < height + 1; i++)
    {
        mvaddch(y + i, x, ACS_VLINE);
        mvaddch(y + i, x + width * 2 + 1, ACS_VLINE);
    }
    mvaddch(y + height + 1, x, ACS_LLCORNER);
    mvaddch(y + height + 1, x + width * 2 + 1, ACS_LRCORNER);
    for (int i = 1; i < width * 2 + 1; i++)
    {
        mvaddch(y + height + 1, x + i, ACS_HLINE);
    }
    attroff(COLOR_PAIR(COLOR_GROUND));
}

void restart_game()
{
    snake_head.x = 0;
    snake_head.y = 0;
    snake_dir.x = 1;
    snake_dir.y = 0;
    snake_score = 0;
    current_frame_time = INITIAL_FRAME_TIME;
    last_speed_increase = 0;
    obstacle_count = 0;
    enemy_count = 0;
    enemy_spawn_counter = 0;
    sprintf(score_message, "[ Score: %d ]", snake_score);
    is_running = true;
}

void process_input_snake()
{
    int pressed = getch();
    if (pressed == KEY_LEFT || pressed == 'a' || pressed == 'A')
    {
        if (snake_dir.x == 1)
        {
            skip = true;
            return;
        }
        snake_dir.x = -1;
        snake_dir.y = 0;
    }
    if (pressed == KEY_RIGHT || pressed == 'd' || pressed == 'D')
    {
        if (snake_dir.x == -1)
        {
            skip = true;
            return;
        }
        snake_dir.x = 1;
        snake_dir.y = 0;
    }
    if (pressed == KEY_UP || pressed == 'w' || pressed == 'W')
    {
        if (snake_dir.y == 1)
        {
            skip = true;
            return;
        }
        snake_dir.x = 0;
        snake_dir.y = -1;
    }
    if (pressed == KEY_DOWN || pressed == 's' || pressed == 'S')
    {
        if (snake_dir.y == -1)
        {
            skip = true;
            return;
        }
        snake_dir.x = 0;
        snake_dir.y = 1;
    }
    if (pressed == ' ')
    {
        if (!is_running)
            restart_game();
    }
    if (pressed == 'q' || pressed == 'Q')
    {
        is_running = false;
    }
}

void game_over_snake()
{
    while (is_running == false)
    {
        process_input_snake();

        attron(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
        mvaddstr(screen_height / 2, screen_width - 16, "         Game Over!        ");
        attroff(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);

        mvaddstr(screen_height / 2 + 1, screen_width - 16, "[SPACE] restart, [Q] quit ");

        attron(COLOR_PAIR(COLOR_GROUND));
        draw_border_snake(screen_height / 2 - 1, screen_width - 17, 17, 2);
        attroff(COLOR_PAIR(COLOR_GROUND));

        usleep(current_frame_time);
        refresh();
    }
}

void update_snake()
{
    for (int i = snake_score; i > 0; i--)
    {
        segments[i] = segments[i - 1];
    }
    segments[0] = snake_head;

    snake_head.x += snake_dir.x;
    snake_head.y += snake_dir.y;

    // murgulduh nuhtsul
    if (collide_snake_body(snake_head) || snake_head.x < 0 || snake_head.y < 0 ||
        snake_head.x >= screen_width || snake_head.y >= screen_height ||
        collide_obstacle(snake_head) || collide_enemy(snake_head))
    {
        is_running = false;
        game_over_snake();
    }

    if (collide(snake_head, snake_berry))
    {
        if (snake_score < MAX_SCORE)
        {
            snake_score += 1;
            sprintf(score_message, "[ Score: %d ]", snake_score);

            // jims ideh uyd hana spawn hiih
            spawn_obstacle();

            // hurd usuh nuhtsul
            if (snake_score % 5 == 0 && snake_score != last_speed_increase)
            {
                last_speed_increase = snake_score;
                if (current_frame_time > MIN_FRAME_TIME)
                {
                    current_frame_time -= 15000;
                }
            }
        }
        snake_berry = spawn_berry();
    }

    // secund bolgon daisan gargaj ireh
    enemy_spawn_counter++;
    if (enemy_spawn_counter >= (1000000 / current_frame_time))
    {
        enemy_spawn_counter = 0;
        spawn_enemy();
    }

    // enemy huuh nuhtsul
    update_enemies();

    usleep(current_frame_time);
}

void draw_snake_game()
{
    erase();

    attron(COLOR_PAIR(COLOR_FOOD) | A_BOLD);
    mvaddch(snake_berry.y + 1, snake_berry.x * 2 + 1, '@');
    attroff(COLOR_PAIR(COLOR_FOOD) | A_BOLD);
    attron(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
    for (int i = 0; i < obstacle_count; i++)
    {
        if (obstacles[i].active)
        {
            mvaddch(obstacles[i].pos.y + 1, obstacles[i].pos.x * 2 + 1, '#');
        }
    }
    attroff(COLOR_PAIR(COLOR_OBSTACLE) | A_BOLD);
    attron(COLOR_PAIR(COLOR_SPIKE) | A_BOLD);
    for (int i = 0; i < enemy_count; i++)
    {
        if (enemies[i].active)
        {
            mvaddch(enemies[i].pos.y + 1, enemies[i].pos.x * 2 + 1, 'X');
        }
    }
    attroff(COLOR_PAIR(COLOR_SPIKE) | A_BOLD);
    attron(COLOR_PAIR(COLOR_PLAYER));
    for (int i = 0; i < snake_score; i++)
    {
        mvaddch(segments[i].y + 1, segments[i].x * 2 + 1, ACS_DIAMOND);
    }
    attroff(COLOR_PAIR(COLOR_PLAYER));

    attron(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);
    mvaddch(snake_head.y + 1, snake_head.x * 2 + 1, 'O');
    attroff(COLOR_PAIR(COLOR_PLAYER) | A_BOLD);

    draw_border_snake(0, 0, screen_width, screen_height);

    attron(COLOR_PAIR(COLOR_MENU));
    mvaddstr(0, screen_width - 5, score_message);
    char speed_msg[32];
    sprintf(speed_msg, "Speed: %d", (INITIAL_FRAME_TIME - current_frame_time) / 15000 + 1);
    mvaddstr(screen_height + 2, 0, speed_msg);
    sprintf(speed_msg, "Enemies: %d | Obstacles: %d", enemy_count, obstacle_count);
    mvaddstr(screen_height + 3, 0, speed_msg);
    attroff(COLOR_PAIR(COLOR_MENU));

    refresh();
}

void playSnake()
{
    srand(time(NULL));

    snake_head.x = 0;
    snake_head.y = 0;
    snake_dir.x = 1;
    snake_dir.y = 0;
    snake_score = 0;
    current_frame_time = INITIAL_FRAME_TIME;
    last_speed_increase = 0;
    obstacle_count = 0;
    enemy_count = 0;
    enemy_spawn_counter = 0;
    is_running = true;
    skip = false;

    snake_berry.x = rand() % screen_width;
    snake_berry.y = rand() % screen_height;

    sprintf(score_message, "[ Score: %d ]", snake_score);

    clear();
    attron(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(0, 0, "SNAKE - Eat food and grow!");
    attroff(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(2, 0, "Use WASD or Arrow Keys, Q to quit, SPACE to restart");
    mvprintw(3, 0, "Speed increases every 5 points!");
    mvprintw(4, 0, "Watch out for obstacles (#) and enemies (X)!");
    refresh();
    msleep(3000);

    timeout(0);
    
    while (is_running)
    {
        process_input_snake();
        if (skip == true)
        {
            skip = false;
            continue;
        }

        update_snake();
        draw_snake_game();
    }

    timeout(-1);
}