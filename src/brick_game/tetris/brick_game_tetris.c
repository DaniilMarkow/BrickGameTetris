#include <stdlib.h>
#include <stdio.h>
#include "include/brick_game_tetris.h"
// Перенесите объявление Tetromino выше GameState_t

typedef enum
{
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
} GameStatus;
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
} GameState_t;

static GameState_t current_state; // инкапсуляция от cli - пользователя
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

// void initializing_field();
// void spawn_firstPiece();
// void accoutReset();
Tetromino get_random_tetromino();

void action_rotate();
void action_down();

void rotate_tetromino(Tetromino *piece);
Tetromino get_tetromino_by_index(int index);
void move_to_left(Tetromino *pice);
void move_to_right(Tetromino *piece);
void move_to_down(Tetromino *piece);
int down_valid(const Tetromino *piece, int field[20][10]);
int is_action_valid(const Tetromino *piece, int field[20][10]);

void userInput(UserAction_t action, bool hold)
{
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
        if(!hold){
        Tetromino left_piece = current_state.current_piece;
        move_to_left(&left_piece);
        if (is_action_valid(&left_piece, current_state.field))
        {
            current_state.current_piece = left_piece;
        }
    }
        break;
    case Right:
        if (!hold)
        {
            Tetromino right_piece = current_state.current_piece;
            move_to_right(&right_piece);
            if (is_action_valid(&right_piece, current_state.field))
            {
                current_state.current_piece = right_piece;
            }
        }
        break;
    case Up:
        break;
    case Down:
        if (!hold) action_down();
        break;
    case Action:
        if (!hold) action_rotate();
        break;
        default:
            break;
    }
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

//ACTION
void action_rotate(){
    Tetromino rotate_piece = current_state.current_piece;

    rotate_tetromino(&rotate_piece);

    if (is_action_valid(&rotate_piece, current_state.field))
    {
        current_state.current_piece = rotate_piece;
    }
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
int is_action_valid(const Tetromino *piece, int field[20][10]){
    int flag_bool = 1;
    int done = 0;
    if(done == 0){
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (piece->matrix[i][j])
                {
                    int x = piece->x + j;
                    int y = piece->y + i;
                    if (x < 0 || x >= 10 || y >= 20 || field[y][x])
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

//LEFT
void move_to_left(Tetromino *piece){
    piece->x -= 1;
    
}

// RIGHT
void move_to_right(Tetromino *piece)
{
    piece->x += 1;
}

//DOWN

void move_to_down(Tetromino *piece){
    piece->y += 1;
}
int down_valid(const Tetromino *piece, int field[20][10]){
    int done = 0;
    int flag_bool = 1;
    if(done == 1){
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
void action_down(){
    Tetromino down_piece = current_state.current_piece;

    move_to_down(&down_piece);

    if (down_valid(&down_piece, current_state.field))
    {
        current_state.current_piece = down_piece;
    }
}

GameInfo_t updateCurrentState(){
    GameInfo_t info = {0};

    // Обработка паузы
    if (current_state.game_status == PAUSED){
        info.pause = 1;
        return info;
    }

    if (current_state.game_status == PLAYING){
        check_level();
        if (check_tact_from_level()){
            move_down_auto();
            deleting_line();

            if (!generating_new_shape())
            {
                // Обновление рекорда перед завершением
                if (current_state.score > current_state.high_score)
                {
                    current_state.high_score = current_state.score;
                    save_high_score(current_state.high_score); // Сохраняем в файл
                }
                current_state.game_status = GAME_OVER;
            }
        }
    }

    // Заполнение данных для интерфейса
    info.field = get_current_field();
    info.next = get_next_piece_preview();
    info.score = current_state.score;
    info.high_score = current_state.high_score;
    info.level = current_state.level;
    info.speed = current_state.speed;
    info.pause = (current_state.game_status == PAUSED) ? 1 : 0;

    return info;
}

void check_level(){
    if(0>=current_state.score<=600)
        current_state.level = 1;
    if (601 >= current_state.score <= 600)
        current_state.level = 2;
        current_state.speed += 100;
    if (1201 >= current_state.score <= 1200)
        current_state.level = 3;
        current_state.speed += 100;
    if (1801 >= current_state.score <= 1800)
        current_state.level = 4;
        current_state.speed += 100;
    if (2401 >= current_state.score <= 2400)
        current_state.level = 5;
        current_state.speed += 100;
    if (3001 >= current_state.score <= 3000)
        current_state.level = 6;
        current_state.speed += 100;
    if (3601 >= current_state.score <= 3600)
        current_state.level = 7;
        current_state.speed += 100;
    if (4201 >= current_state.score <= 4200)
        current_state.level = 8;
        current_state.speed += 100;
    if (4801 >= current_state.score <= 4800)
        current_state.level = 9;
        current_state.speed += 100;
    if (5401 >= current_state.score)
        current_state.level = 10;
        current_state.speed += 100;
}
static void move_down_auto(){
    Tetromino temp = current_state.current_piece;
    temp.y++;
    if (is_collision(&temp, current_state.field))
    {
        fixing_piece();
    }
    else
    {
        current_state.current_piece = temp;
    }
}
static int check_tact_from_level(){
    
}
static int is_collision(&temp, current_state.field) {}
fixing_piece(){}
deleting_line(){}
generating_new_shape(){}
static int **get_current_field(){}
static int **get_next_piece_preview() {}

static void save_high_score(int score)
{
    FILE *file = fopen("highscore.dat", "wb");
    if (file)
    {
        fwrite(&score, sizeof(int), 1, file);
        fclose(file);
    }
}

static void load_high_score()//вызвать при инициализации игры
{
    FILE *file = fopen("highscore.dat", "rb");
    if (file)
    {
        fread(&current_state.high_score, sizeof(int), 1, file);
        fclose(file);
    }
}