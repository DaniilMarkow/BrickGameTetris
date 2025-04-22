#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "include/brick_game_tetris.h"

#define WIDTH 10
#define HEIGHT 20

static const Tetromino TETROMINOES[] = {
    {.type = I, .matrix = {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, .color_pair = 1},
    {.type = J, .matrix = {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}}, .color_pair = 2},
    {.type = L, .matrix = {{0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}}, .color_pair = 3},
    {.type = O, .matrix = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}, .color_pair = 4},
    {.type = S, .matrix = {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}}, .color_pair = 5},
    {.type = T, .matrix = {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}}, .color_pair = 6},
    {.type = Z, .matrix = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}}, .color_pair = 7}};

int **copy_tetromino(Tetromino piece);
int **copy_field(int field[HEIGHT][WIDTH]);
void initializing_game(GameState *state);
Tetromino get_tetromino_by_index(int index);
Tetromino get_random_tetromino();
void action_rotate(GameState *state);
void rotate_tetromino(Tetromino *piece);
int is_action_valid(const Tetromino *piece, int field[HEIGHT][WIDTH]);
void move_to_left(Tetromino *piece);
void move_to_right(Tetromino *piece);
int down_valid(const Tetromino *piece, int field[HEIGHT][WIDTH]);
void action_down(GameState *state);
int check_level(GameState *state);
int generating_new_shape(GameState *state);
static void move_down_auto(GameState *state);
static void fixing_piece(GameState *state);
static void load_high_score(GameState *state);
static int check_tact_from_level(GameState *state);
static void deleting_line(GameState *state);

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
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            state->field[i][j] = 0;

    state->score = 0;
    state->level = 1;
    state->speed = 500;
    state->game_status = PLAYING;
    state->current_piece = get_random_tetromino();
    state->next_piece = get_random_tetromino();
    load_high_score(state);
    state->total_lines_cleared = 0;
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

void action_rotate(GameState *state)
{
    Tetromino rotate_piece = state->current_piece;
    rotate_tetromino(&rotate_piece);

    if (is_action_valid(&rotate_piece, state->field))
    {
        state->current_piece = rotate_piece;
        return;
    }

    int offsets[] = {-1, 1, -2, 2};
    for (int i = 0; i < 4; i++)
    {
        Tetromino test_piece = rotate_piece;
        test_piece.x += offsets[i];
        if (is_action_valid(&test_piece, state->field))
        {
            state->current_piece = test_piece;
            return;
        }
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
                if (x < 0 || x >= WIDTH || y < 0 || y > HEIGHT - 1)
                {
                    return 0;
                }
                if (field[y][x] != 0)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

void move_to_left(Tetromino *piece)
{
    piece->x -= 1;
}

void move_to_right(Tetromino *piece)
{
    piece->x += 1;
}

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
                    if (y + 1 >= HEIGHT || field[y + 1][x])
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
    else
    {
        fixing_piece(state);
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
    int **new_field = (int **)malloc(HEIGHT * sizeof(int *));
    for (int i = 0; i < HEIGHT; i++)
    {
        new_field[i] = (int *)malloc(WIDTH * sizeof(int));
        for (int j = 0; j < WIDTH; j++)
        {
            new_field[i][j] = field[i][j];
        }
    }
    return new_field;
}

void free_field(int **field)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        free(field[i]);
    }
    free(field);
}

int **copy_tetromino(Tetromino piece)
{
    int **next_piece = (int **)malloc(4 * sizeof(int *));
    for (int i = 0; i < 4; i++)
    {
        next_piece[i] = (int *)malloc(4 * sizeof(int));
        for (int j = 0; j < 4; j++)
        {
            next_piece[i][j] = piece.matrix[i][j];
        }
    }
    return next_piece;
}

void free_next_piece(int **next_piece)
{
    for (int i = 0; i < 4; i++)
    {
        free(next_piece[i]);
    }
    free(next_piece);
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
    state->current_piece.x = (WIDTH - 4) / 2;
    state->current_piece.y = 0;

    if (!is_action_valid(&state->current_piece, state->field))
    {
        printf("Cannot place new piece. Game Over!\n");
        return 0;
    }

    state->next_piece = get_random_tetromino();
    state->total_pieces_placed++;
    return 1;
}

static void move_down_auto(GameState *state)
{
    Tetromino temp = state->current_piece;
    temp.y++;
    if (!is_action_valid(&temp, state->field))
    {
        fixing_piece(state);
    }
    else
    {
        state->current_piece = temp;
    }
}

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
        last_fall = now;
    }

    int current_delay_ms = 1000 - (state->level * 50);
    if (current_delay_ms < 100)
        current_delay_ms = 100;

    long required_us = (long)current_delay_ms * 1000;
    long elapsed_us = (now.tv_sec - last_fall.tv_sec) * 1000000L + (now.tv_usec - last_fall.tv_usec);

    if (elapsed_us >= required_us)
    {
        last_fall = now;
        return 1;
    }
    return 0;
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

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (piece->matrix[i][j])
            {
                int x = piece->x + j;
                int y = piece->y + i;

                if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
                {
                    state->field[y][x] = piece->type + 1; // Сохраняем тип фигуры (I=1, J=2, ..., Z=7)
                }
            }
        }
    }

    if (!generating_new_shape(state))
    {
        state->game_status = GAME_OVER;
    }
}