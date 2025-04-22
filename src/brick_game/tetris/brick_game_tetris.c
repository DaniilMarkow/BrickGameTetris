#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h> // Для работы с clock() и CLOCKS_PER_SEC
#include "include/brick_game_tetris.h"
// Перенесите объявление Tetromino выше GameState_t
#define WIDTH 10
#define HEIGHT 20

static const Tetromino TETROMINOES[] = {
    {.type = I, .matrix = {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
    {.type = J, .matrix = {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}}},
    {.type = L, .matrix = {{0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}}},
    {.type = O, .matrix = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}},
    {.type = S, .matrix = {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}}},
    {.type = T, .matrix = {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}}},
    {.type = Z, .matrix = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}}}};

int **copy_tetromino(Tetromino piece);
// void free_next_piece(int **next_piece);
int **copy_field(int field[HEIGHT][WIDTH]);
void initializing_game(GameState *state);
Tetromino get_tetromino_by_index(int index);
Tetromino get_random_tetromino();
void action_rotate(GameState *state);
void rotate_tetromino(Tetromino *piece);
int is_action_valid(const Tetromino *piece, int field[HEIGHT][WIDTH]);
void move_to_left(Tetromino *piece);
void move_to_right(Tetromino *piece);
// void move_to_down(Tetromino *piece);
int down_valid(const Tetromino *piece, int field[HEIGHT][WIDTH]);
void action_down(GameState *state);
// GameInfo_t updateCurrentState();
// void free_field(int **field);
int check_level(GameState *state);
int generating_new_shape(GameState *state);
static void move_down_auto(GameState *state);
// static int is_collision(Tetromino *piece, int field[HEIGHT][WIDTH]);
static void fixing_piece(GameState *state);
// void save_high_score(int score);
static void load_high_score(GameState *state);
static int check_tact_from_level(GameState *state);
static void deleting_line(GameState *state);
// static int is_game_over(int field[HEIGHT][WIDTH]);

void userInput(UserAction_t action, bool hold, GameState *state)
{
    switch (action)
    {
    case Start:
        initializing_game(state);
        break;
    case Pause:
        if (state->game_status == PLAYING)
            state->game_status = PAUSED;
        else if (state->game_status == PAUSED)
            state->game_status = PLAYING;
        break;
    case Terminate:
        state->game_status = GAME_OVER;
        break;
    case Left:
        if (!hold)
        {
            Tetromino left = state->current_piece;
            move_to_left(&left);
            if (is_action_valid(&left, state->field))
                state->current_piece = left;
        }
        break;
    case Right:
        if (!hold)
        {
            Tetromino right = state->current_piece;
            move_to_right(&right);
            if (is_action_valid(&right, state->field))
                state->current_piece = right;
        }
        break;
    case Up:
        break;
    case Down:
        if (!hold)
            action_down(state);
        break;
    case Action:
        if (!hold)
            action_rotate(state);
        break;
    default:
        break;
    }
}
GameState *createGameState()
{
    GameState *state = malloc(sizeof(GameState));
    initializing_game(state);
    return state;
}

void destroyGameState(GameState *state)
{
    free(state);
}
void initializing_game(GameState *state)
{
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 10; j++)
            state->field[i][j] = 0;

    state->score = 0;
    state->level = 1;
    state->speed = 500;
    state->game_status = PLAYING;
    state->current_piece = get_random_tetromino();
    state->next_piece = get_random_tetromino();
    load_high_score(state);
    state->total_lines_cleared = 0; // Инициализация
    state->total_pieces_placed = 0;
}
Tetromino get_tetromino_by_index(int index)
{
    return TETROMINOES[index];
}

Tetromino get_random_tetromino()
{
    int index = rand() % 7;
    return TETROMINOES[index];
}
//

// ACTION
void action_rotate(GameState *state)
{
    Tetromino rotate_piece = state->current_piece;

    rotate_tetromino(&rotate_piece);

    if (is_action_valid(&rotate_piece, state->field))
    {
        state->current_piece = rotate_piece;
    }
}
void rotate_tetromino(Tetromino *piece)
{
    int temp[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            temp[i][j] = piece->matrix[j][i];
        }
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            piece->matrix[i][j] = temp[i][3 - j];
        }
    }
}
// int is_action_valid(const Tetromino *piece, int field[HEIGHT][WIDTH])
// {
//     int flag_bool = 1;
//     int done = 0;
//     if (done == 0)
//     {
//         for (int i = 0; i < 4; i++)
//         {
//             for (int j = 0; j < 4; j++)
//             {
//                 if (piece->matrix[i][j])
//                 {
//                     int x = piece->x + j;
//                     int y = piece->y + i;
//                     if (x < 0 || x >= WIDTH || y >= HEIGHT || field[y][x])
//                     {
//                         flag_bool = 0;
//                         done = 1;
//                     }
//                 }
//             }
//         }
//     }
//     return flag_bool;
// }
int is_action_valid(const Tetromino *piece, int field[HEIGHT][WIDTH])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (piece->matrix[i][j])
            {
                int x = piece->x + j;
                int y = piece->y + i;
                // Сначала проверяем границы
                if (x < 0 || x >= WIDTH || y < 0 || y > HEIGHT - 1)
                {
                    return 0;
                }
                // Затем безопасно проверяем пересечение
                if (field[y][x] != 0)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

// LEFT
void move_to_left(Tetromino *piece)
{
    piece->x -= 1;
}

// RIGHT
void move_to_right(Tetromino *piece)
{
    piece->x += 1;
}

// DOWN

// void move_to_down(Tetromino *piece)
// {
//     piece->y += 1;
// }
int down_valid(const Tetromino *piece, int field[HEIGHT][WIDTH])
{
    int done = 0;
    int flag_bool = 1;
    if (done == 1)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (piece->matrix[i][j])
                {
                    int x = piece->x + j;
                    int y = piece->y + i;
                    if (y + 1 >= 20 || field[y + 1][x])
                    {
                        flag_bool = 0;
                        done = 1;
                    }
                }
            }
        }
    }
    return flag_bool;
}
void action_down(GameState *state)
{
    Tetromino down_piece = state->current_piece;
    down_piece.y += 1;
    if (is_action_valid(&down_piece, state->field))
    {
        state->current_piece = down_piece;
    }
    //вроде и без этого норм работало
    else
    {
        fixing_piece(state); // Фиксируем фигуру, если движение вниз невозможно
    }
}

GameInfo_t updateCurrentState(GameState *state)
{
    GameInfo_t info = {0};
    if (state->game_status == PLAYING)
    {
        check_level(state);
        if (check_tact_from_level(state))
        {
            move_down_auto(state);
            deleting_line(state);
        }
    }

    info.field = copy_field(state->field);
    info.next = copy_tetromino(state->next_piece);
    info.score = state->score;
    info.high_score = state->high_score;
    info.level = state->level;
    info.speed = state->speed;
    info.pause = (state->game_status == PAUSED) ? 1 : 0;

    return info;
}

int **copy_field(int field[HEIGHT][WIDTH])
{
    int **new_field = (int **)malloc(20 * sizeof(int *));
    for (int i = 0; i < 20; i++)
    {
        new_field[i] = (int *)malloc(10 * sizeof(int));
        for (int j = 0; j < 10; j++)
        {
            new_field[i][j] = field[i][j];
        }
    }
    return new_field;
}

void free_field(int **field)
{
    for (int i = 0; i < 20; i++)
    {
        free(field[i]);
    }
    free(field);
}
int **copy_tetromino(Tetromino piece)
{
    int **next_piece = (int **)malloc(4 * sizeof(int *)); // Allocate rows
    for (int i = 0; i < 4; i++)
    {
        next_piece[i] = (int *)malloc(4 * sizeof(int)); // Allocate columns
        for (int j = 0; j < 4; j++)
        {
            next_piece[i][j] = piece.matrix[i][j]; // Copy data from Tetromino matrix
        }
    }
    return next_piece;
}

void free_next_piece(int **next_piece)
{
    for (int i = 0; i < 4; i++)
    {
        free(next_piece[i]); // Free each row
    }
    free(next_piece); // Free the array of pointers
}

int check_level(GameState *state)
{
    int new_level = state->level;

    if (state->score >= 5400)
        new_level = 10;
    else if (state->score >= 4800)
        new_level = 9;
    else if (state->score >= 4200)
        new_level = 8;
    else if (state->score >= 3600)
        new_level = 7;
    else if (state->score >= 3000)
        new_level = 6;
    else if (state->score >= 2400)
        new_level = 5;
    else if (state->score >= 1800)
        new_level = 4;
    else if (state->score >= 1200)
        new_level = 3;
    else if (state->score >= 600)
        new_level = 2;
    else
        new_level = 1;

    // Увеличиваем скорость только при повышении уровня
    if (new_level > state->level)
    {
        state->speed += 100;
    }

    state->level = new_level;
    return state->level;
}

int generating_new_shape(GameState *state)
{
   

    state->current_piece = state->next_piece;
    state->current_piece.x = (WIDTH - 4) / 2;        // Центрирование по ширине
    state->current_piece.y = 0;                      // Начальная позиция сверху

    if (!is_action_valid(&state->current_piece, state->field))
    {
        printf("Cannot place new piece. Game Over!\n");
        return 0; // Игра окончена
    }

    state->next_piece = get_random_tetromino();
    state->total_pieces_placed++;
    return 1;
}
// int generating_new_shape(GameState *state)
// {
//     state->current_piece = state->next_piece;
//     state->next_piece = get_random_tetromino();
//     state->current_piece.x = WIDTH / 2 - 2;
//     state->current_piece.y = 0;

//     // Проверяем, можно ли разместить фигуру на стартовой позиции
//     if (!is_action_valid(&(state->current_piece), state->field))
//     {
//         // Если нельзя, проверяем, заполнена ли верхняя строка
//         if (is_game_over(state->field))
//         {
//             return 0; // Игра окончена
//         }
//         // Если верхняя строка не заполнена, пытаемся сместить фигуру вниз
//         for (int y = 1; y < HEIGHT; y++)
//         {
//             state->current_piece.y = y;
//             if (is_action_valid(&(state->current_piece), state->field))
//             {
//                 break; // Нашли допустимую позицию
//             }
//         }
//         // Если не нашли допустимую позицию до конца поля
//         if (state->current_piece.y >= HEIGHT - 1)
//         {
//             return 0; // Игра окончена
//         }
//     }
//     state->total_pieces_placed++;
//     return 1;
// }

static void move_down_auto(GameState *state)
{
    Tetromino temp = state->current_piece;
    temp.y++;
    if (!is_action_valid(&temp, state->field)) // Если движение вниз невозможно
    {
        fixing_piece(state);
    }
    else
    {
        state->current_piece = temp;
    }
}

// static int is_collision(Tetromino *piece, int field[HEIGHT][WIDTH])
// {
//     int flag_bool = 0;
//     int done = 0;

//     if (done == 0)
//     {
//         for (int i = 0; i < 4; i++)
//         {
//             for (int j = 0; j < 4; j++)
//             {
//                 if (piece->matrix[i][j] == 1)
//                 {
//                     int x = piece->x + j;
//                     int y = piece->y + i;

//                     if (x < 0 || x >= 10 || y < 0 || y >= 20)
//                     {
//                         flag_bool = 1;
//                         done = 1;
//                     }
//                     if (field[y][x] != 0)
//                     {
//                         flag_bool = 1;
//                         done = 1;
//                     }
//                 }
//             }
//         }
//     }
//     return flag_bool;
// }

void save_high_score(int score)
{
    FILE *file = fopen("highscore.dat", "wb");
    if (file)
    {
        fwrite(&score, sizeof(int), 1, file);
        fclose(file);
    }
}

static void load_high_score(GameState *state)
{
    FILE *file = fopen("highscore.dat", "rb");
    if (file)
    {
        fread(&state->high_score, sizeof(int), 1, file);
        fclose(file);
    }
    else
    {
        // Если файл не существует, устанавливаем рекорд в 0
        state->high_score = 0;
    }
}

static int check_tact_from_level(GameState *state)
{
    static struct timeval last_fall = {0, 0};
    struct timeval now;
    gettimeofday(&now, NULL);

    if (last_fall.tv_sec == 0 && last_fall.tv_usec == 0)
    {
        last_fall = now; // Инициализация на первом вызове
    }

    int current_delay_ms = 1000 - (state->level * 50);
    if (current_delay_ms < 100)
        current_delay_ms = 100;

    long required_us = (long)current_delay_ms * 1000; // Перевод ms в микросекунды
    long elapsed_us = (now.tv_sec - last_fall.tv_sec) * 1000000L + (now.tv_usec - last_fall.tv_usec);

    if (elapsed_us >= required_us)
    {
        last_fall = now;
        return 1; // Время падения пришло
    }
    return 0; // Еще не время
}

static void deleting_line(GameState *state)
{
    int lines_cleared = 0;

    for (int y = HEIGHT - 1; y >= 0; y--)
    {
        int line_complete = 1;
        for (int x = 0; x < WIDTH; x++)
        {
            if (state->field[y][x] == 0)
            {
                line_complete = 0;
                break;
            }
        }

        if (line_complete)
        {
            lines_cleared++;
            for (int ny = y; ny > 0; ny--)
            {
                for (int x = 0; x < WIDTH; x++)
                {
                    state->field[ny][x] = state->field[ny - 1][x];
                }
            }
            for (int x = 0; x < WIDTH; x++)
                state->field[0][x] = 0;
            y++;
        }
    }

    if (lines_cleared > 0)
    {
        switch (lines_cleared)
        {
        case 1:
            state->score += 100;
            break;
        case 2:
            state->score += 300;
            break;
        case 3:
            state->score += 700;
            break;
        case 4:
            state->score += 1500;
            break;
        }
        if (state->score > state->high_score)
        {
            state->high_score = state->score;
        }
        check_level(state);
        state->total_lines_cleared += lines_cleared;
    }
}

static void fixing_piece(GameState *state)
{
    Tetromino *piece = &(state->current_piece);

    // Фиксация фигуры на поле
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (piece->matrix[i][j])
            {
                int x = piece->x + j;
                int y = piece->y + i;

                // Проверка границ и запись в поле
                if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
                {
                    state->field[y][x] = 1; // Используем state->field вместо current_state.field
                }
            }
        }
    }

    // Генерация новой фигуры и проверка завершения игры
    if (!generating_new_shape(state))
    { // Передаем state в generating_new_shape
        state->game_status = GAME_OVER;
    }
}


// static int is_game_over(int field[HEIGHT][WIDTH])
// {
//     // Проверяем верхнюю строку (y = 0)
//     for (int x = 0; x < WIDTH; x++)
//     {
//         if (field[0][x] == 0) // Если есть хотя бы одна свободная клетка
//         {
//             return 0;
//         }
//     }
//     return 1; // Верхняя строка полностью заполнена
// }