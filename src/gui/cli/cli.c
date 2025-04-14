#include <ncurses.h>
#include <stdlib.h>
#include "../../brick_game/tetris/include/brick_game_tetris.h"

#define FIELD_COLOR 1
#define FIGURE_COLOR 2
#define SIDEBAR_COLOR 3

void init_colors()
{
    start_color();
    init_pair(FIELD_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(FIGURE_COLOR, COLOR_CYAN, COLOR_BLACK);
    init_pair(SIDEBAR_COLOR, COLOR_YELLOW, COLOR_BLACK);
}

void draw_game(WINDOW *win, const GameInfo_t *info)
{
    werase(win);
    box(win, 0, 0);

    // Отрисовка игрового поля
    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            if (info->field[y][x])
            {
                wattron(win, COLOR_PAIR(FIELD_COLOR));
                mvwaddch(win, y + 1, x * 2 + 1, '[');
                mvwaddch(win, y + 1, x * 2 + 2, ']');
                wattroff(win, COLOR_PAIR(FIELD_COLOR));
            }
        }
    }
    wrefresh(win);
}

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    init_colors();

    WINDOW *game_win = newwin(22, 22, 1, 1);
    GameInfo_t game_info = updateCurrentState();

    bool running = true;
    while (running)
    {
        int ch = getch();
        switch (ch)
        {
        case KEY_LEFT:
            userInput(Left, false);
            break;
        case KEY_RIGHT:
            userInput(Right, false);
            break;
        case KEY_DOWN:
            userInput(Down, false);
            break;
        case KEY_UP:
            userInput(Action, false);
            break;
        case 'q':
            running = false;
            break;
        }

        game_info = updateCurrentState();
        draw_game(game_win, &game_info);
        napms(100);
    }

    endwin();
    return 0;
}