// brick_game_tetris.h
#ifndef BRICK_GAME_TETRIS_H
#define BRICK_GAME_TETRIS_H

#include <stdbool.h>

typedef enum
{
    Start,
    Pause,
    Terminate,
    Left,
    Right,
    Up,
    Down,
    Action
} UserAction_t;

typedef struct
{
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;

// Объявления структур, используемых в API
typedef enum
{
    I,
    J,
    L,
    O,
    S,
    T,
    Z
} TetrominoType;

typedef struct
{
    int x, y;
    TetrominoType type;
    int matrix[4][4];
} Tetromino;

// Публичные функции
void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();
void rotate_tetromino(Tetromino *piece);
Tetromino get_tetromino_by_index(int index);

#endif