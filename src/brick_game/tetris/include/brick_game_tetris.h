#ifndef BRICK_GAME_TETRIS_H
#define BRICK_GAME_TETRIS_H

#include <stdbool.h>
#include <stdlib.h>

// Состояния игры
typedef enum
{
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
} GameStatus;

// Типы тетрамино
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

// Структура тетрамино
typedef struct
{
    int x, y;
    TetrominoType type;
    int matrix[4][4];
} Tetromino;

// Состояние игры
typedef struct
{
    GameStatus game_status;
    Tetromino current_piece;
    Tetromino next_piece;
    int field[20][10];
    int score;
    int high_score;
    int level;
    int speed;
    int total_lines_cleared; // Добавлено
    int total_pieces_placed;
} GameState;

// Информация для интерфейса
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

// Действия пользователя
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

// Публичные функции
GameState *createGameState();
void destroyGameState(GameState *state);
void userInput(UserAction_t action, bool hold, GameState *state);
GameInfo_t updateCurrentState(GameState *state);
void free_field(int **field);
void free_next_piece(int **next_piece);
void save_high_score(int score);
#endif