#include <stdio.h>
#include "include/brick_game_tetris.h" // Подключаем заголовочный файл с логикой игры

// Функция для отображения игрового поля в CLI
void display_field(int field[20][10]) {
    printf("--------------------\n");
    for (int i = 0; i < 20; i++) {
        printf("|");
        for (int j = 0; j < 10; j++) {
            if (field[i][j] == 0) {
                printf(" "); // Пустая клетка
            } else {
                printf("*"); // Занятая клетка
            }
        }
        printf("|\n");
    }
    printf("--------------------\n");
}

int main() {
    // Инициализация игры (например, вызов userInput(Start, false))
    userInput(Start, false);

    // Основной цикл игры
    while (current_state.game_status == PLAYING) {
        // 1. Получаем ввод пользователя (пример: 'a' - влево, 'd' - вправо, 's' - вниз)
        char input;
        printf("Enter action (a=left, d=right, s=down, r=rotate, q=quit): ");
        scanf(" %c", &input);

        UserAction_t action;
        switch (input) {
            case 'a': action = Left; break;
            case 'd': action = Right; break;
            case 's': action = Down; break;
            case 'r': action = Action; break;
            case 'q': action = Terminate; break;
            default: action = Start; break;
        }

        // 2. Обрабатываем ввод пользователя
        userInput(action, false); // false - означает, что клавиша не удерживается

        // 3. Обновляем состояние игры
        GameInfo_t game_info = updateCurrentState();

        // 4. Отображаем игру в CLI
        display_field(game_info.field);
        printf("Score: %d, Level: %d\n", game_info.score, game_info.level);
        free_field(game_info.field); // Освобождаем память

        // Дополнительно: показываем следующую фигуру (game_info.next)
        // ...
    }

    printf("Game Over!\n");
    return 0;
}
