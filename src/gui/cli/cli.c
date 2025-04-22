// #include "../../brick_game/tetris/include/brick_game_tetris.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <termios.h>
// #include <string.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <time.h>



// #define WIDTH 10  // Added missing definition

// // Terminal control functions
// static struct termios old_termios, new_termios;

// void init_termios(int echo) {
//     tcgetattr(0, &old_termios);
//     new_termios = old_termios;
//     new_termios.c_lflag &= ~ICANON;
//     new_termios.c_lflag &= echo ? ECHO : ~ECHO;
//     tcsetattr(0, TCSANOW, &new_termios);
// }

// void reset_termios(void) {
//     tcsetattr(0, TCSANOW, &old_termios);
// }

// char getch_(int echo) {
//     char ch;
//     init_termios(echo);
//     ch = getchar();
//     reset_termios();
//     return ch;
// }

// char getch(void) {
//     return getch_(0);
// }

// int kbhit(void) {
//     struct termios oldt, newt;
//     int ch;
//     int oldf;

//     tcgetattr(STDIN_FILENO, &oldt);
//     newt = oldt;
//     newt.c_lflag &= ~(ICANON | ECHO);
//     tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//     oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
//     fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

//     ch = getchar();

//     tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//     fcntl(STDIN_FILENO, F_SETFL, oldf);

//     if (ch != EOF) {
//         ungetc(ch, stdin);
//         return 1;
//     }

//     return 0;
// }

// // Game drawing function
// void draw_game(const GameInfo_t* game_info) {
//     system("clear");

//     printf("Score: %d\n", game_info->score);
//     printf("High Score: %d\n", game_info->high_score);
//     printf("Level: %d\n", game_info->level);
//     printf("Speed: %d\n\n", game_info->speed);

//     // Создаем временную копию поля для временного размещения текущей фигуры
//     int temp_field[20][10];
//     memcpy(temp_field, game_info->field, sizeof(temp_field)); // Скопировали оригинальное поле

//     // Временное наложение текущей фигуры на поле
//     for (int i = 0; i < 4; ++i) {
//         for (int j = 0; j < 4; ++j) {
//             if (current_state.current_piece.matrix[i][j]) { // Обращаемся к глобальной переменной current_state
//                 int x = current_state.current_piece.x + j;
//                 int y = current_state.current_piece.y + i;

//                 // Если координаты внутри границ поля
//                 if (x >= 0 && x < WIDTH && y >= 0 && y < 20) {
//                     temp_field[y][x] = current_state.current_piece.matrix[i][j];
//                 }
//             }
//         }
//     }

//     // Отображение поля с наложенной фигурой
//     for (int y = 0; y < 20; y++) {
//         printf("|");
//         for (int x = 0; x < WIDTH; x++) {
//             printf(temp_field[y][x] ? "#" : " ");
//         }
//         printf("|\n");
//     }
//     printf("+----------+\n");

//     // Отображаем следующую фигуру
//     printf("\nNext piece:\n");
//     for (int y = 0; y < 4; y++) {
//         for (int x = 0; x < 4; x++) {
//             printf(game_info->next[y][x] ? "#" : " ");
//         }
//         printf("\n");
//     }

//     if (game_info->pause) {
//         printf("\nPAUSED\nPress 'p' to continue\n");
//     }
// }


// // Main game loop
// void game_loop() {
//     GameInfo_t game_info;
//     bool running = true;
    
//     // Initialize game
//     userInput(Start, false);
    
//     while (running) {
//         // Handle user input
//         if (kbhit()) {
//             char c = getch();
//             switch (c) {
//                 case 'q':
//                     userInput(Terminate, false);
//                     running = false;
//                     break;
//                 case 'p':
//                     userInput(Pause, false);
//                     break;
//                 case 'a':
//                     userInput(Left, false);
//                     break;
//                 case 'd':
//                     userInput(Right, false);
//                     break;
//                 case 's':
//                     userInput(Down, false);
//                     break;
//                 case ' ':
//                     userInput(Action, false);
//                     break;
//             }
//         }
        
//         // Update game state
//         game_info = updateCurrentState();
        
//         // Draw game
//         draw_game(&game_info);
        
//         // Check game over
//         if (game_info.field == NULL) {
//             running = false;
//         }
        
//         // Small delay
//         usleep(10000);
//     }
    
//     // Clean up
//     if (game_info.field != NULL) {
//         for (int i = 0; i < 20; i++) {
//             free(game_info.field[i]);
//         }
//         free(game_info.field);
//     }
    
//     if (game_info.next != NULL) {
//         free(game_info.next);
//     }
    
//     printf("Game Over! Final Score: %d\n", game_info.score);
// }

// int main() {
//     srand(time(NULL));
    
//     printf("Tetris Game\n");
//     printf("Controls:\n");
//     printf("a - Move Left\n");
//     printf("d - Move Right\n");
//     printf("s - Move Down\n");
//     printf("space - Rotate\n");
//     printf("p - Pause\n");
//     printf("q - Quit\n\n");
//     printf("Press any key to start...");
//     getch();
    
//     game_loop();
    
//     return 0;
// }