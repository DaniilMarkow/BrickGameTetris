#include "gui.h"

void init_colors() {
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);     // I
  init_pair(2, COLOR_BLUE, COLOR_BLACK);     // J
  init_pair(3, COLOR_WHITE, COLOR_BLACK);    // L
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);   // O
  init_pair(5, COLOR_GREEN, COLOR_BLACK);    // S
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);  // T
  init_pair(7, COLOR_RED, COLOR_BLACK);      // Z
}

void draw_game(WINDOW *win, const GameInfo_t *info,
               const Tetromino *current_piece) {
  // Отрисовка игрового поля
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (info->field[y][x] != 0) {
        wattron(win, COLOR_PAIR(info->field[y][x]));
        mvwaddch(win, y + 1, x + 1, '#');
        wattroff(win, COLOR_PAIR(info->field[y][x]));
      } else {
        mvwaddch(win, y + 1, x + 1, ' ');
      }
    }
  }

  // Отрисовка текущей фигуры
  for (int r = 0; r < 4; r++) {
    for (int c = 0; c < 4; c++) {
      if (current_piece->matrix[r][c] == 1) {
        int py = current_piece->y + r;
        int px = current_piece->x + c;
        if (py >= 0 && py < HEIGHT && px >= 0 && px < WIDTH) {
          wattron(win, COLOR_PAIR(current_piece->color_pair));
          mvwaddch(win, py + 1, px + 1, '#');
          wattroff(win, COLOR_PAIR(current_piece->color_pair));
        }
      }
    }
  }
}

void draw_next_piece(WINDOW *win, const Tetromino *piece) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (piece->matrix[i][j]) {
        wattron(win, COLOR_PAIR(piece->color_pair));
        mvwaddch(win, i + 1, j + 1, '#');
        wattroff(win, COLOR_PAIR(piece->color_pair));
      } else {
        mvwaddch(win, i + 1, j + 1, ' ');
      }
    }
  }
}

void draw_info(WINDOW *win, const GameInfo_t *info, int total_lines_cleared,
               int total_pieces_placed) {
  mvwprintw(win, 1, 1, "Score: %d", info->score);
  mvwprintw(win, 2, 1, "Level: %d", info->level);
  mvwprintw(win, 3, 1, "High Score: %d", info->high_score);
  mvwprintw(win, 4, 1, "Lines Cleared: %d", total_lines_cleared);
  mvwprintw(win, 5, 1, "Pieces Placed: %d", total_pieces_placed);
  mvwprintw(win, 6, 1, "Speed: %d", info->speed);
}