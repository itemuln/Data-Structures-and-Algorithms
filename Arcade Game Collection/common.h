#ifndef COMMON_H
#define COMMON_H

#include <ncurses.h>

#define WIDTH 50
#define HEIGHT 20
#define MAX_OBSTACLES 100

// Color pairs
#define COLOR_PLAYER 1
#define COLOR_OBSTACLE 2
#define COLOR_FOOD 3
#define COLOR_GROUND 4
#define COLOR_MENU 5
#define COLOR_SPIKE 6
#define COLOR_PIPE 7

typedef struct Obstacle
{
    int x, y;
    char symbol;
    struct Obstacle *next;
} Obstacle;

Obstacle *createObstacle(int x, int y, char symbol);
void freeObstacles(Obstacle *head);
void msleep(int milliseconds);
void initColors();
void playSound(const char *soundName);
void playBackgroundMusic(const char *trackName);
void stopBackgroundMusic();

#endif
