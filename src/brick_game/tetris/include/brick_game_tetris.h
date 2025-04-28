#ifndef BRICK_GAME_TETRIS_H
#define BRICK_GAME_TETRIS_H
#include <stdbool.h>
#include <stdlib.h>
#define HEIGHT 20
#define WIDTH 10

typedef enum { I, J, L, O, S, T, Z } TetrominoType;

typedef struct {
  TetrominoType type;
  int matrix[4][4];
  int x;
  int y;
  int color_pair;
} Tetromino;

typedef struct {
  int field[HEIGHT][WIDTH];
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

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef enum { PLAYING, PAUSED, GAME_OVER } GameStatus;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  int fall_delay;
} GameInfo_t;

// Декларации функций, которые уже были
GameState *createGameState();
void destroyGameState(GameState *state);
void userInput(UserAction_t action, bool hold, GameState *state);
GameInfo_t updateCurrentState(GameState *state);
void free_field(int **field);
void free_next_piece(int **next);
void save_high_score(int score);

// Добавленные декларации для тестируемых функций
void initializing_game(GameState *state);
Tetromino get_tetromino_by_index(int index);
Tetromino get_random_tetromino();
void rotate_tetromino(Tetromino *piece);
int is_action_valid(const Tetromino *piece, const int field[HEIGHT][WIDTH]);
void move_to_left(Tetromino *piece);
void move_to_right(Tetromino *piece);
void action_down(GameState *state);
int **copy_field(int field[HEIGHT][WIDTH]);
int **copy_tetromino(Tetromino piece);
int check_level(GameState *state);
int generating_new_shape(GameState *state);
void action_rotate(GameState *state);

// Функции, которые были static, теперь доступны для тестов
void move_down_auto(GameState *state);
void fixing_piece(GameState *state);
void load_high_score(GameState *state);
int check_tact_from_level(const GameState *state);
void deleting_line(GameState *state);

#endif