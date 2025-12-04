#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "common.h"

Obstacle *createObstacle(int x, int y, char symbol)
{
    Obstacle *obs = (Obstacle *)malloc(sizeof(Obstacle));
    obs->x = x;
    obs->y = y;
    obs->symbol = symbol;
    obs->next = NULL;
    return obs;
}

void freeObstacles(Obstacle *head)
{
    while (head != NULL)
    {
        Obstacle *temp = head;
        head = head->next;
        free(temp);
    }
}

void msleep(int milliseconds)
{
    usleep(milliseconds * 1000);
}

void initColors()
{
    if (has_colors())
    {
        start_color();
        init_pair(COLOR_PLAYER, COLOR_CYAN, COLOR_BLACK);
        init_pair(COLOR_OBSTACLE, COLOR_RED, COLOR_BLACK);
        init_pair(COLOR_FOOD, COLOR_GREEN, COLOR_BLACK);
        init_pair(COLOR_GROUND, COLOR_YELLOW, COLOR_BLACK);
        init_pair(COLOR_MENU, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(COLOR_SPIKE, COLOR_RED, COLOR_BLACK);
        init_pair(COLOR_PIPE, COLOR_GREEN, COLOR_BLACK);
    }
}

void playSound(const char *soundName)
{
    if (strcmp(soundName, "jump") == 0)
    {
        printf("\a");
        fflush(stdout);
    }
    else if (strcmp(soundName, "collision") == 0)
    {
        printf("\a\a");
        fflush(stdout);
    }
    else if (strcmp(soundName, "score") == 0)
    {
        printf("\a");
        fflush(stdout);
    }
}

void playBackgroundMusic(const char *trackName)
{
    char command[256];
    snprintf(command, sizeof(command), "afplay -t 999999 sounds/%s.mp3 &", trackName);
    system(command);
}

void stopBackgroundMusic()
{
    system("killall afplay 2>/dev/null");
}
