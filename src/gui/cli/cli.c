#include <ncurses.h>
#include "../../brick_game/tetris/include/brick_game_tetris.h"

void draw_field(int field[20][10], Tetromino *piece)
{
    WINDOW *game_win = newwin(22, 22, 0, 0);
    box(game_win, 0, 0); // Рисуем рамку

    // Отрисовка поля
    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            if (field[y][x])
            {
                mvwprintw(game_win, y + 1, x * 2 + 1, "[]");
            }
        }
    }

    // Отрисовка фигуры
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (piece->matrix[i][j])
            {
                int px = piece->x + j;
                int py = piece->y + i;
                if (py >= 0 && py < 20 && px >= 0 && px < 10)
                {
                    mvwprintw(game_win, py + 1, px * 2 + 1, "{}");
                }
            }
        }
    }

    wrefresh(game_win); // Обновляем окно поля
    delwin(game_win);
    refresh(); // Обновляем основной экран
}

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    if (LINES < 22 || COLS < 22)
    {
        endwin();
        printf("Увеличьте размер терминала (минимум 22x22)\n");
        return 1;
    }

    int field[20][10] = {0};
    Tetromino test_piece = get_tetromino_by_index(1);
    test_piece.x = 3;
    test_piece.y = 0;

    // Первая отрисовка
    draw_field(field, &test_piece);
    refresh(); // Явное обновление основного экрана

    int ch;
    while ((ch = getch()) != 'q')
    {
        if (ch == KEY_UP)
        {
            rotate_tetromino(&test_piece);
        }
        draw_field(field, &test_piece);
        napms(100);
    }

    endwin();
    return 0;
}