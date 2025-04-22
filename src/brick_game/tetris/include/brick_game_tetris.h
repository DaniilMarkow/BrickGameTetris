#ifndef BRICK_GAME_TETRIS_H
#define BRICK_GAME_TETRIS_H
#include <stdbool.h>
#include <stdlib.h>
#include <ncurses.h>
#define HEIGHT 20
#define WIDTH 10

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
    TetrominoType type;
    int matrix[4][4];
    int x;
    int y;
    int color_pair; // Добавляем поле для цветовой пары
} Tetromino;

typedef struct
{
    int field[HEIGHT][WIDTH]; // Теперь поле хранит TetrominoType (0 для пустоты, 1-7 для типов фигур)
    Tetromino current_piece;
    Tetromino next_piece;
    int score;
    int high_score;
    int level;
    int speed;
    int game_status;
    int total_lines_cleared;
    int total_pieces_placed;
} GameState;

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

typedef enum
{
    PLAYING,
    PAUSED,
    GAME_OVER
} GameStatus;

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

GameState *createGameState();
void destroyGameState(GameState *state);
void userInput(UserAction_t action, bool hold, GameState *state);
GameInfo_t updateCurrentState(GameState *state);
void free_field(int **field);
void free_next_piece(int **next);
void save_high_score(int score);

#endif