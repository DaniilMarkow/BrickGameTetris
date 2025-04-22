#include "../../brick_game/tetris/include/brick_game_tetris.h"
#include <ncurses.h>

#define WIDTH 10
#define HEIGHT 20

// Функция для запуска игрового цикла
void run_game(GameState *state, WINDOW *game_win, WINDOW *info_win, WINDOW *next_win, WINDOW *controls_win)
{
    while (state->game_status != GAME_OVER)
    {
        // Обновить состояние игры
        GameInfo_t info = updateCurrentState(state);

        // Помечаем окна как измененные вместо полной очистки
        touchwin(game_win);
        touchwin(info_win);
        touchwin(next_win);
        touchwin(controls_win);

        // Нарисовать рамки
        box(game_win, 0, 0);
        box(info_win, 0, 0);
        box(next_win, 0, 0);
        box(controls_win, 0, 0);

        // Нарисовать игровое поле на game_win
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                mvwaddch(game_win, i + 1, j + 1, state->field[i][j] ? '#' : ' ');
            }
        }
        // Нарисовать текущую фигуру
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
                        mvwaddch(game_win, py + 1, px + 1, '#');
                    }
                }
            }
        }
        // Если на паузе, нарисовать сообщение
        if (state->game_status == PAUSED)
        {
            mvwprintw(game_win, 10, 1, "Paused - Press 'p' to resume");
        }

        // Нарисовать следующую фигуру на next_win
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                mvwaddch(next_win, i + 1, j + 1, state->next_piece.matrix[i][j] ? '#' : ' ');
            }
        }

        // Нарисовать информацию на info_win
        mvwprintw(info_win, 1, 1, "Score: %d", state->score);
        mvwprintw(info_win, 2, 1, "Level: %d", state->level);
        mvwprintw(info_win, 3, 1, "High Score: %d", state->high_score);
        mvwprintw(info_win, 4, 1, "Lines Cleared: %d", state->total_lines_cleared);
        mvwprintw(info_win, 5, 1, "Pieces Placed: %d", state->total_pieces_placed);

        // Нарисовать управление на controls_win
        mvwprintw(controls_win, 1, 1, "Controls: Left/Right: A/D or Arrows, Down: S, Rotate: W or Up, Pause: P, Quit: Q");

        // Обновить окна
        wrefresh(game_win);
        wrefresh(info_win);
        wrefresh(next_win);
        wrefresh(controls_win);

        // Получить ввод пользователя
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

        // Освободить память
        free_field(info.field);
        free_next_piece(info.next);

        // Задержка
        napms(50);
    }
}

int main()
{
    // Инициализация ncurses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    timeout(0);
    curs_set(0);

    bool play_game = true;
    while (play_game)
    {
        // Создать состояние игры
        GameState *state = createGameState();

        // Создать окна
        WINDOW *game_win = newwin(22, 12, 0, 0); // Возвращаем 22 строки
        WINDOW *info_win = newwin(7, 22, 0, 12);
        WINDOW *next_win = newwin(6, 6, 7, 12);
        WINDOW *controls_win = newwin(3, 42, 22, 0); // Смещено под game_win

        // Запустить игровой цикл
        run_game(state, game_win, info_win, next_win, controls_win);

        // Удалить окна
        delwin(game_win);
        delwin(info_win);
        delwin(next_win);
        delwin(controls_win);

        // Сохранить рекорд
        save_high_score(state->high_score);

        // Создать окно для результатов
        WINDOW *game_over_win = newwin(10, 40, 5, 5);
        box(game_over_win, 0, 0);

        // Отобразить результаты
        mvwprintw(game_over_win, 1, 2, "Game Over!");
        mvwprintw(game_over_win, 2, 2, "Final Score: %d", state->score);
        mvwprintw(game_over_win, 3, 2, "High Score: %d", state->high_score);
        mvwprintw(game_over_win, 4, 2, "Level: %d", state->level);
        mvwprintw(game_over_win, 5, 2, "Lines Cleared: %d", state->total_lines_cleared);
        mvwprintw(game_over_win, 6, 2, "Pieces Placed: %d", state->total_pieces_placed);
        mvwprintw(game_over_win, 8, 2, "[Retry (R)]  [Quit (Q)]");

        wrefresh(game_over_win);

        // Ожидать выбора пользователя
        int choice;
        bool game_over = true;
        while (game_over)
        {
            choice = getch();
            if (choice == 'r' || choice == 'R')
            {
                // Очистить окно результатов
                delwin(game_over_win);
                clear();
                refresh();

                // Уничтожить текущее состояние игры
                destroyGameState(state);

                // Перезапустить игру
                game_over = false; // Выходим из цикла game_over, внешний цикл перезапустит игру
            }
            else if (choice == 'q' || choice == 'Q')
            {
                // Завершить игру
                delwin(game_over_win);
                play_game = false; // Выходим из внешнего цикла
                game_over = false;
            }
        }
    }

    // Завершить ncurses
    endwin();

    return 0;
}