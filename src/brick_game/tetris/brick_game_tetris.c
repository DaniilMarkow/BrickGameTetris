#include <stdlib.h>
#include <stdio.h>
#include <time.h> // Для работы с clock() и CLOCKS_PER_SEC
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
        initializing_game();
        break;
    case Pause:
    if (current_state.game_status == PLAYING) {
                current_state.game_status = PAUSED;
            } else if (current_state.game_status == PAUSED) {
                current_state.game_status = PLAYING;
            }
            break;
    case Terminate:
    current_state.game_status = GAME_OVER;
            break;
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
void initializing_game() {
    // Сброс поля
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            current_state.field[i][j] = 0;
        }
    }

    // Сброс счета и уровня
    current_state.score = 0;
    current_state.level = 1;
    current_state.speed = 500;

    // Установка статуса игры
    current_state.game_status = PLAYING;

    // Создание первой фигуры и следующей фигуры
    current_state.current_piece = get_random_tetromino();
    current_state.next_piece = get_random_tetromino();

    // Загрузка рекорда
    load_high_score();
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

GameInfo_t updateCurrentState() {
    GameInfo_t info = {0};

    if (current_state.game_status == PLAYING) {
        check_level();
        if (check_tact_from_level()) {
            move_down_auto(); // Здесь вызывается fixing_piece() при коллизии
            deleting_line();

            // Проверка game_over происходит внутри fixing_piece()
        }
    }

    // Заполнение данных для интерфейса
    info.field = copy_field(current_state.field); // Выделяем память и копируем поле
    info.next = copy_tetromino(current_state.next_piece); // Копируем структуру Tetromino
    info.score = current_state.score;
    info.high_score = current_state.high_score;
    info.level = current_state.level;
    info.speed = current_state.speed;
    info.pause = (current_state.game_status == PAUSED) ? 1 : 0;

    return info;
}

int **copy_field(int field[20][10]){
    int **new_field = (int**)malloc(20*sizeof(int*));
    for (int i = 0; i < 20; i++){
        new_field[i] = (int*)malloc(10*sizeof(int));
        for (int j = 0; j < 10; j++){
            new_field[i][j] = field[i][j];
        }
    } 
    return new_field;
}

void free_field(int** field) {
    for (int i = 0; i < 20; i++) {
        free(field[i]);
    }
    free(field);
}
Tetromino* copy_tetromino(Tetromino piece) {
    Tetromino* new_piece = (Tetromino*)malloc(sizeof(Tetromino));
    *new_piece = piece;
    return new_piece;
}

int check_level() {
    int new_level = current_state.level;
    
    if (current_state.score >= 5400) new_level = 10;
    else if (current_state.score >= 4800) new_level = 9;
    else if (current_state.score >= 4200) new_level = 8;
    else if (current_state.score >= 3600) new_level = 7;
    else if (current_state.score >= 3000) new_level = 6;
    else if (current_state.score >= 2400) new_level = 5;
    else if (current_state.score >= 1800) new_level = 4;
    else if (current_state.score >= 1200) new_level = 3;
    else if (current_state.score >= 600) new_level = 2;
    else new_level = 1;
    
    // Увеличиваем скорость только при повышении уровня
    if (new_level > current_state.level) {
        current_state.speed += 100;
    }
    
    current_state.level = new_level;
    return current_state.level;
}

int generating_new_shape() {
    // Копируем следующую фигуру в текущую
    current_state.current_piece = current_state.next_piece;

    // Сбрасываем координаты в начальное положение (центр верхнего края)
    current_state.current_piece.x = (WIDTH - 4) / 2; // Центрирование для матрицы 4x4
    current_state.current_piece.y = 0;

    // Проверяем коллизию новой фигуры
    if (is_collision(&current_state.current_piece, current_state.field) {
        return 0; // Невозможно разместить -> игра окончена
    }

    // Генерируем следующую фигуру для preview
    current_state.next_piece = get_random_tetromino();
    return 1;
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

static int is_collision(Tetromino *piece, int field[20][10]) {
    int flag_bool = 0;
    int done = 0;

    if(done == 0){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if(piece->matrix[i][j] == 1){
                int x = piece->x+j;
                int y = piece->y+i;

                if (x < 0 || x >= 10 || y < 0 || y >=20){
                    flag_bool = 1;
                    done = 1;
                }
                if (field[y][x] != 0){
                    flag_bool = 1;
                    done = 1;
                }
            }
        }
    }
}
return flag_bool;
}
static void fixing_piece(){
    Tetromino *piece = &current_state.current_piece;

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if(piece->matrix[i][j] == 1){
                int x = piece->x +j;
                int y = piece ->y + i;

                if (x>=0 && x < 10 && y >= 0 && y < 20){
                    current_state.field[y][x] = 1;
                }
            }
        }
    }
    if(!generating_new_shape()){
        current_state.game_status = GAME_OVER;
    }
}

static void save_high_score(int score)
{
    FILE *file = fopen("highscore.dat", "wb");
    if (file)
    {
        fwrite(&score, sizeof(int), 1, file);
        fclose(file);
    }
}

static void load_high_score() {
    FILE *file = fopen("highscore.dat", "rb");
    if (file) {
        fread(&current_state.high_score, sizeof(int), 1, file);
        fclose(file);
    } else {
        // Если файл не существует, устанавливаем рекорд в 0
        current_state.high_score = 0;
    }
}




// static int check_tact_from_level() {

//     int is_down = 0;
//     int done = 0;
//     if (done != 1){
//     // Статическая переменная для хранения времени последнего падения
//     static clock_t last_fall = 0;
    
//     // Текущее время в тактах процессора
//     clock_t now = clock();
    
//     // Базовая задержка между падениями (в миллисекундах)
//     int base_delay_ms = 1000; // 1 секунда для уровня 1
    
//     // Рассчитываем задержку в зависимости от уровня
//     int current_delay_ms = base_delay_ms - (current_state.level * 50);
    
//     // Минимальная задержка (чтобы игра не стала невозможной)
//     if (current_delay_ms < 100) {
//         current_delay_ms = 100; // Нижний порог: 100 мс
//     }
    
//     // Переводим задержку в такты процессора
//     clock_t required_ticks = (current_delay_ms * CLOCKS_PER_SEC) / 1000;
    
//     // Проверяем, прошло ли достаточно времени
//     if (now - last_fall >= required_ticks) {
//         last_fall = now; // Обновляем время последнего падения
//         is_down = 1; // Да, фигура должна упасть
//         done = 1;
//     }
//     }
//     return is_down; // Нет, время ещё не пришло
// }
static int check_tact_from_level() {
    static clock_t last_fall = 0;
    clock_t now = clock();
    
    int current_delay_ms = 1000 - (current_state.level * 50);
    if (current_delay_ms < 100) current_delay_ms = 100;
    
    clock_t required_ticks = (current_delay_ms * CLOCKS_PER_SEC) / 1000;
    
    if (now - last_fall >= required_ticks) {
        last_fall = now;
        return 1;
    }
    return 0;
}

static void deleting_line() {
    int lines_cleared = 0;
    
    // Проверяем поле снизу вверх (от 19 строки к 0)
    for (int y = 19; y >= 0; y--) {
        int line_complete = 1;
        
        // Проверяем, заполнена ли текущая строка
        for (int x = 0; x < 10; x++) {
            if (current_state.field[y][x] == 0) {
                line_complete = 0;
                break;
            }
        }
        
        // Если строка заполнена
        if (line_complete) {
            lines_cleared++;
            
            // Сдвигаем все строки выше текущей вниз
            for (int ny = y; ny > 0; ny--) {
                for (int x = 0; x < 10; x++) {
                    current_state.field[ny][x] = current_state.field[ny-1][x];
                }
            }
            
            // Очищаем самую верхнюю строку
            for (int x = 0; x < 10; x++) {
                current_state.field[0][x] = 0;
            }
            
            // Повторно проверяем текущую строку (так как в нее переместилась строка сверху)
            y++;
        }
    }
    
    // Начисляем очки за удаленные линии
    if (lines_cleared > 0) {
        switch (lines_cleared) {
            case 1: current_state.score += 100; break;
            case 2: current_state.score += 300; break;
            case 3: current_state.score += 700; break;
            case 4: current_state.score += 1500; break;
        }
        
        // Обновляем рекорд, если нужно
        if (current_state.score > current_state.high_score) {
            current_state.high_score = current_state.score;
        }
        
        // Проверяем повышение уровня
        check_level();
    }
}