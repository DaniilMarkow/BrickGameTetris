#include "../../brick_game/tetris/include/brick_game_tetris.h"
#include <ncurses.h>

#define WIDTH 10
#define HEIGHT 20

void run_game(GameState *state, WINDOW *game_win, WINDOW *info_win, WINDOW *next_win, WINDOW *controls_win)
{
    while (state->game_status != GAME_OVER)
    {
        GameInfo_t info = updateCurrentState(state);

        touchwin(game_win);
        touchwin(info_win);
        touchwin(next_win);
        touchwin(controls_win);

        box(game_win, 0, 0);
        box(info_win, 0, 0);
        box(next_win, 0, 0);
        box(controls_win, 0, 0);

        // Отрисовка игрового поля с цветами
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                if (state->field[i][j] != 0)
                {
                    wattron(game_win, COLOR_PAIR(state->field[i][j]));
                    mvwaddch(game_win, i + 1, j + 1, '#');
                    wattroff(game_win, COLOR_PAIR(state->field[i][j]));
                }
                else
                {
                    mvwaddch(game_win, i + 1, j + 1, ' ');
                }
            }
        }

        // Отрисовка текущей фигуры с цветом
        for (int r = 0; r < 4; r++)
        {
            for (int c = 0; c < 4; c++)
            {
                if (state->current_piece.matrix[r][c] == 1)
                {
                    int py = state->current_piece.y + r;
                    int px = state->current_piece.x + c;
                    if (py >= 0 && py < HEIGHT && px >= 0 && px < WIDTH)
                    {
                        wattron(game_win, COLOR_PAIR(state->current_piece.color_pair));
                        mvwaddch(game_win, py + 1, px + 1, '#');
                        wattroff(game_win, COLOR_PAIR(state->current_piece.color_pair));
                    }
                }
            }
        }

        if (state->game_status == PAUSED)
        {
            mvwprintw(game_win, 10, 1, "Paused - Press 'p' to resume");
        }

        // Отрисовка следующей фигуры с цветом
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (state->next_piece.matrix[i][j])
                {
                    wattron(next_win, COLOR_PAIR(state->next_piece.color_pair));
                    mvwaddch(next_win, i + 1, j + 1, '#');
                    wattroff(next_win, COLOR_PAIR(state->next_piece.color_pair));
                }
                else
                {
                    mvwaddch(next_win, i + 1, j + 1, ' ');
                }
            }
        }

        // Отображение информации, включая скорость
        mvwprintw(info_win, 1, 1, "Score: %d", state->score);
        mvwprintw(info_win, 2, 1, "Level: %d", state->level);
        mvwprintw(info_win, 3, 1, "High Score: %d", state->high_score);
        mvwprintw(info_win, 4, 1, "Lines Cleared: %d", state->total_lines_cleared);
        mvwprintw(info_win, 5, 1, "Pieces Placed: %d", state->total_pieces_placed);
        mvwprintw(info_win, 6, 1, "Speed: %d", state->speed); // Добавляем отображение скорости

        mvwprintw(controls_win, 1, 1, "Controls: Left/Right: A/D or Arrows, Down: S, Rotate: W or Up, Pause: P, Quit: Q");

        wrefresh(game_win);
        wrefresh(info_win);
        wrefresh(next_win);
        wrefresh(controls_win);

        int ch = getch();
        UserAction_t action;
        bool has_action = false;

        switch (ch)
        {
        case KEY_LEFT:
        case 'a':
            action = Left;
            has_action = true;
            break;
        case KEY_RIGHT:
        case 'd':
            action = Right;
            has_action = true;
            break;
        case KEY_DOWN:
        case 's':
            action = Down;
            has_action = true;
            break;
        case KEY_UP:
        case 'w':
            action = Action;
            has_action = true;
            break;
        case 'p':
            action = Pause;
            has_action = true;
            break;
        case 'q':
            action = Terminate;
            has_action = true;
            break;
        }

        if (has_action)
        {
            userInput(action, false, state);
        }

        free_field(info.field);
        free_next_piece(info.next);

        napms(50);
    }
}

int main()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    timeout(0);
    curs_set(0);

    if (has_colors() == FALSE)
    {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_GREEN, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_RED, COLOR_BLACK);

    bool play_game = true;
    while (play_game)
    {
        GameState *state = createGameState();

        WINDOW *game_win = newwin(22, 12, 0, 0);
        WINDOW *info_win = newwin(7, 22, 0, 12);
        WINDOW *next_win = newwin(6, 6, 7, 12);
        WINDOW *controls_win = newwin(3, 42, 22, 0);

        run_game(state, game_win, info_win, next_win, controls_win);

        delwin(game_win);
        delwin(info_win);
        delwin(next_win);
        delwin(controls_win);

        save_high_score(state->high_score);

        WINDOW *game_over_win = newwin(10, 40, 5, 5);
        box(game_over_win, 0, 0);

        mvwprintw(game_over_win, 1, 2, "Game Over!");
        mvwprintw(game_over_win, 2, 2, "Final Score: %d", state->score);
        mvwprintw(game_over_win, 3, 2, "High Score: %d", state->high_score);
        mvwprintw(game_over_win, 4, 2, "Level: %d", state->level);
        mvwprintw(game_over_win, 5, 2, "Lines Cleared: %d", state->total_lines_cleared);
        mvwprintw(game_over_win, 6, 2, "Pieces Placed: %d", state->total_pieces_placed);
        mvwprintw(game_over_win, 8, 2, "[Retry (R)]  [Quit (Q)]");

        wrefresh(game_over_win);

        
        bool game_over = true;
        while (game_over)
        {
            int choice;
            choice = getch();
            if (choice == 'r' || choice == 'R')
            {
                delwin(game_over_win);
                clear();
                refresh();
                destroyGameState(state);
                game_over = false;
            }
            else if (choice == 'q' || choice == 'Q')
            {
                delwin(game_over_win);
                play_game = false;
                game_over = false;
            }
        }
    }

    endwin();
    return 0;
}