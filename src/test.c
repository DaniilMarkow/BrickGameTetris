#include "../../brick_game/tetris/include/brick_game_tetris.h"

int main() {
    // Initialize game state for testing
    initializing_game();

    // Test move_down_auto()
    printf("Testing move_down_auto...\n");
    move_down_auto();

    // Print current piece position after moving down
    printf("Current piece position: x = %d, y = %d\n", current_state.current_piece.x, current_state.current_piece.y);

    // Test fixing_piece()
    printf("Testing fixing_piece...\n");
    fixing_piece();

    // Print field state after fixing piece
    printf("Field state:\n");
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%d ", current_state.field[i][j]);
        }
        printf("\n");
    }

    return 0;
}
