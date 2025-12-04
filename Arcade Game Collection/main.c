#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "common.h"
#include "temple_run.h"
#include "geometry_dash.h"
#include "flappy_bird.h"
#include "snake.h"
#include "dino_run.h"

void displayMenu()
{
    clear();
    attron(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(0, 0, "╔════════════════════════════════════════════╗");
    mvprintw(1, 0, "║        ARCADE GAME COLLECTION             ║");
    mvprintw(2, 0, "╠════════════════════════════════════════════╣");
    attroff(COLOR_PAIR(COLOR_MENU) | A_BOLD);

    attron(COLOR_PAIR(COLOR_PLAYER));
    mvprintw(3, 0, "║  1. Starship Shooter                      ║");
    mvprintw(4, 0, "║  2. Rhythm Platformer                     ║");
    mvprintw(5, 0, "║  3. Flappy Bird                           ║");
    mvprintw(6, 0, "║  4. Snake                                 ║");
    mvprintw(7, 0, "║  5. Dino Run                              ║");
    attroff(COLOR_PAIR(COLOR_PLAYER));

    attron(COLOR_PAIR(COLOR_OBSTACLE));
    mvprintw(8, 0, "║  6. Exit                                  ║");
    attroff(COLOR_PAIR(COLOR_OBSTACLE));

    attron(COLOR_PAIR(COLOR_MENU) | A_BOLD);
    mvprintw(9, 0, "╚════════════════════════════════════════════╝");
    attroff(COLOR_PAIR(COLOR_MENU) | A_BOLD);

    mvprintw(10, 0, "Enter your choice: ");
    refresh();
}

int main()
{
    srand(time(NULL));

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    initColors();

    int choice;

    while (1)
    {
        displayMenu();
        echo();
        scanw("%d", &choice);
        noecho();

        switch (choice)
        {
        case 1:
            playTempleRun();
            break;
        case 2:
            playGeometryDash();
            break;
        case 3:
            playFlappyBird();
            break;
        case 4:
            playSnake();
            break;
        case 5:
            playDinoRun();
            break;
        case 6:
            clear();
            mvprintw(HEIGHT / 2, WIDTH / 2 - 10, "Thanks for playing! Goodbye!");
            refresh();
            msleep(1500);
            endwin();
            return 0;
        default:
            mvprintw(12, 0, "Press any key to continue...");
            refresh();
            getch();
        }
    }

    endwin();
    return 0;
}