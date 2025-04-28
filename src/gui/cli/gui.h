#ifndef GUI_H
#define GUI_H

#include <ncurses.h>

#include "../../brick_game/tetris/include/brick_game_tetris.h"

void init_colors();
void draw_game(WINDOW *win, const GameInfo_t *info,
               const Tetromino *current_piece);
void draw_next_piece(WINDOW *win, const Tetromino *piece);
void draw_info(WINDOW *win, const GameInfo_t *info, int total_lines_cleared,
               int total_pieces_placed);

#endif