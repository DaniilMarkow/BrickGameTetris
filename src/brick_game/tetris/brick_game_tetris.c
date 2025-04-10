#include <stdlib.h>
#include "include/brick_game_tetris.h"
// Перенесите объявление Tetromino выше GameState_t

typedef enum
{
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
} GameStatus;

typedef struct
{
    GameStatus game_status;
    Tetromino current_piece; // Теперь компилятор знает о Tetromino
    Tetromino next_piece;
    int field[20][10];
    int score;
    int high_score;
    int level;
    int speed;
} GameState_t;

static GameState_t current_state; // инкапсуляция от cli - пользователя

// void initializing_field();
// void spawn_firstPiece();
// void accoutReset();


static const Tetromino TETROMINOES[] = {
    {.matrix = {
         {0},
         {1, 1, 1, 1},
         {0},
         {0}},
     .type = I},
    {.matrix = {{0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0}}, .type = J},
    {.matrix = {{0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0}}, .type = L},
    {.matrix = {{0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0}}, .type = O},
    {.matrix = {{0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0}}, .type = S},
    {.matrix = {{0}, {0, 0, 1, 0}, {0, 1, 1, 1}, {0}}, .type = T},
    {.matrix = {{0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0}}, .type = Z}};

Tetromino get_tetromino_by_index(int index)
{
    return TETROMINOES[index];
}

void userInput(UserAction_t action, bool hold){
    switch (action)
    {
    case Start:
        // initializing_field();
        // spawn_firstPiece();
        // accoutReset();
        break;
    case Pause:
    case Terminate:
    case Left:
    case Right:
    case Up:
    case Down:
    case Action:
    default:
        break;
    }
}

Tetromino get_random_tetromino()
{
    int index = rand() % 7;
    return TETROMINOES[index];
}

void rotate_tetromino(Tetromino *piece)
{
    int temp[4][4];
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            temp[i][j] = piece->matrix[j][i];
        }
    }
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            piece->matrix[i][j] = temp[i][3 - j];
        }
    }

}

