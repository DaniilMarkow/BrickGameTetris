#include <ncurses.h>
#include "../../brick_game/tetris/include/brick_game_tetris.h"

// Константы для цветов
#define FIELD_COLOR 1
#define FIGURE_COLOR 2
#define SIDEBAR_COLOR 3

// Инициализация цветов
// void init_colors() {
//     start_color();
//     init_pair(FIELD_COLOR, COLOR_WHITE, COLOR_BLACK);  // Блоки на поле
//     init_pair(FIGURE_COLOR, COLOR_CYAN, COLOR_BLACK);  // Текущая фигура
//     init_pair(SIDEBAR_COLOR, COLOR_YELLOW, COLOR_BLACK); // Боковая панель
// }

// Отрисовка игрового поля
// void draw_game(WINDOW *win, const GameInfo_t *info) {
//     werase(win);
//     box(win, 0, 0);
    
//     // Отрисовка стакана
//     for (int y = 0; y < 20; y++) {
//         for (int x = 0; x < 10; x++) {
//             if (info->field[y][x]) {
//                 wattron(win, COLOR_PAIR(FIELD_COLOR));
//                 mvwaddch(win, y+1, x*2+1, '[');
//                 mvwaddch(win, y+1, x*2+2, ']');
//                 wattroff(win, COLOR_PAIR(FIELD_COLOR));
//             }
//         }
//     }
//     wrefresh(win);
// }

// Отрисовка боковой панели
// void draw_sidebar(WINDOW *win, const GameInfo_t *info) {
//     werase(win);
//     box(win, 0, 0);
    
//     mvwprintw(win, 1, 1, "Score: %d", info->score);
//     mvwprintw(win, 2, 1, "Level: %d", info->level);
//     mvwprintw(win, 3, 1, "High: %d", info->high_score);
    
//     // Отрисовка следующей фигуры
//     mvwprintw(win, 5, 1, "Next:");
//     wattron(win, COLOR_PAIR(SIDEBAR_COLOR));
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++) {
//             if (info->next[i][j]) {
//                 mvwaddch(win, 7+i, 3+j*2, '[');
//                 mvwaddch(win, 7+i, 3+j*2+1, ']');
//             }
//         }
//     }
//     wattroff(win, COLOR_PAIR(SIDEBAR_COLOR));
    
//     // Подсказки управлением
//     mvwprintw(win, 12, 1, "Controls:");
//     mvwprintw(win, 13, 1, "←/→: Move");
//     mvwprintw(win, 14, 1, "↑: Rotate");
//     mvwprintw(win, 15, 1, "↓: Drop");
//     mvwprintw(win, 16, 1, "Q: Quit");
    
//     wrefresh(win);
// }