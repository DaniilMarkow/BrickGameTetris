#include <check.h>
#include <stdlib.h>

#include "../tetris/include/brick_game_tetris.h"

#define WIDTH 10
#define HEIGHT 20

START_TEST(test_initializing_game) {
  GameState *state = createGameState();
  initializing_game(state);

  ck_assert_int_eq(state->score, 0);
  ck_assert_int_eq(state->level, 1);
  ck_assert_int_eq(state->speed, 500);
  ck_assert_int_eq(state->game_status, PLAYING);
  ck_assert_int_eq(state->total_lines_cleared, 0);
  ck_assert_int_eq(state->total_pieces_placed, 0);

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      ck_assert_int_eq(state->field[i][j], 0);
    }
  }

  destroyGameState(state);
}
END_TEST

START_TEST(test_get_tetromino_by_index) {
  Tetromino piece = get_tetromino_by_index(0);  // I-фигура
  ck_assert_int_eq(piece.type, I);
  ck_assert_int_eq(piece.matrix[1][0], 1);
  ck_assert_int_eq(piece.matrix[1][1], 1);
  ck_assert_int_eq(piece.matrix[1][2], 1);
  ck_assert_int_eq(piece.matrix[1][3], 1);
  ck_assert_int_eq(piece.color_pair, 1);
}
END_TEST

START_TEST(test_get_random_tetromino) {
  Tetromino piece = get_random_tetromino();
  ck_assert(piece.type >= I && piece.type <= Z);
}
END_TEST

START_TEST(test_rotate_tetromino) {
  Tetromino piece = get_tetromino_by_index(0);  // I-фигура
  rotate_tetromino(&piece);
  ck_assert_int_eq(piece.matrix[0][2], 1);
  ck_assert_int_eq(piece.matrix[1][2], 1);
  ck_assert_int_eq(piece.matrix[2][2], 1);
  ck_assert_int_eq(piece.matrix[3][2], 1);
}
END_TEST

START_TEST(test_is_action_valid) {
  Tetromino piece = get_tetromino_by_index(0);  // I-фигура
  piece.x = (WIDTH - 4) / 2;
  piece.y = 0;

  int field[HEIGHT][WIDTH] = {0};
  ck_assert_int_eq(is_action_valid(&piece, field), 1);

  // Поместим фигуру за пределы поля по x
  piece.x = -1;
  ck_assert_int_eq(is_action_valid(&piece, field), 0);

  // Поместим фигуру за пределы поля по y
  piece.x = (WIDTH - 4) / 2;
  piece.y = HEIGHT;
  ck_assert_int_eq(is_action_valid(&piece, field), 0);

  // Поместим фигуру на занятую клетку
  piece.x = (WIDTH - 4) / 2;
  piece.y = 0;
  field[1][(WIDTH - 4) / 2] = 1;  // Блок на y=1
  ck_assert_int_eq(is_action_valid(&piece, field), 0);
}
END_TEST

START_TEST(test_move_to_left_right) {
  Tetromino piece = get_tetromino_by_index(0);
  piece.x = (WIDTH - 4) / 2;

  int original_x = piece.x;
  move_to_left(&piece);
  ck_assert_int_eq(piece.x, original_x - 1);

  move_to_right(&piece);
  ck_assert_int_eq(piece.x, original_x);
}
END_TEST

START_TEST(test_action_down) {
  GameState *state = createGameState();
  state->current_piece = get_tetromino_by_index(0);  // I-фигура
  state->current_piece.x = (WIDTH - 4) / 2;
  state->current_piece.y = HEIGHT - 2;  // Блоки на y=HEIGHT-1

  // Заполним строки y=0 и y=1, чтобы новая фигура не могла появиться
  for (int x = 0; x < WIDTH; x++) {
    state->field[0][x] = 1;
    state->field[1][x] = 1;
  }

  action_down(state);
  ck_assert_int_eq(state->game_status, GAME_OVER);

  destroyGameState(state);
}
END_TEST

START_TEST(test_copy_free_field) {
  int field[HEIGHT][WIDTH] = {0};
  field[0][0] = 1;
  int **copied = copy_field(field);
  ck_assert_int_eq(copied[0][0], 1);
  free_field(copied);
}
END_TEST

START_TEST(test_copy_free_tetromino) {
  Tetromino piece = get_tetromino_by_index(0);
  int **copied = copy_tetromino(piece);
  ck_assert_int_eq(copied[1][0], 1);
  free_next_piece(copied);
}
END_TEST

START_TEST(test_check_level) {
  GameState *state = createGameState();
  state->score = 1200;
  check_level(state);
  ck_assert_int_eq(state->level, 3);
  destroyGameState(state);
}
END_TEST

START_TEST(test_generating_new_shape) {
  GameState *state = createGameState();
  state->next_piece = get_tetromino_by_index(0);
  int result = generating_new_shape(state);
  ck_assert_int_eq(result, 1);
  ck_assert_int_eq(state->current_piece.x, (WIDTH - 4) / 2);
  ck_assert_int_eq(state->current_piece.y, 0);

  // Заполним строки y=0 и y=1, чтобы новая фигура не могла появиться
  for (int x = 0; x < WIDTH; x++) {
    state->field[0][x] = 1;
    state->field[1][x] = 1;
  }
  result = generating_new_shape(state);
  ck_assert_int_eq(result, 0);

  destroyGameState(state);
}
END_TEST

START_TEST(test_move_down_auto) {
  GameState *state = createGameState();
  state->current_piece = get_tetromino_by_index(0);
  state->current_piece.x = (WIDTH - 4) / 2;
  state->current_piece.y = HEIGHT - 2;

  // Заполним строки y=0 и y=1, чтобы новая фигура не могла появиться
  for (int x = 0; x < WIDTH; x++) {
    state->field[0][x] = 1;
    state->field[1][x] = 1;
  }

  move_down_auto(state);
  ck_assert_int_eq(state->game_status, GAME_OVER);

  destroyGameState(state);
}
END_TEST

START_TEST(test_fixing_piece) {
  GameState *state = createGameState();
  state->current_piece = get_tetromino_by_index(0);  // I-фигура
  state->current_piece.x = (WIDTH - 4) / 2;
  state->current_piece.y = HEIGHT - 2;  // Блоки на y=HEIGHT-1

  fixing_piece(state);
  int x = (WIDTH - 4) / 2;
  ck_assert_int_eq(state->field[HEIGHT - 1][x], I + 1);
  ck_assert_int_eq(state->field[HEIGHT - 1][x + 1], I + 1);
  ck_assert_int_eq(state->field[HEIGHT - 1][x + 2], I + 1);
  ck_assert_int_eq(state->field[HEIGHT - 1][x + 3], I + 1);

  destroyGameState(state);
}
END_TEST

START_TEST(test_deleting_line) {
  GameState *state = createGameState();
  for (int x = 0; x < WIDTH; x++) {
    state->field[HEIGHT - 1][x] = 1;
  }
  deleting_line(state);
  for (int x = 0; x < WIDTH; x++) {
    ck_assert_int_eq(state->field[HEIGHT - 1][x], 0);
  }
  ck_assert_int_eq(state->score, 100);
  destroyGameState(state);
}
END_TEST

START_TEST(test_load_high_score) {
  GameState *state = createGameState();
  save_high_score(1000);
  load_high_score(state);
  ck_assert_int_eq(state->high_score, 1000);
  destroyGameState(state);
}
END_TEST

START_TEST(test_check_tact_from_level) {
  GameState *state = createGameState();
  state->level = 1;
  int result = check_tact_from_level(state);
  ck_assert(result == 0 || result == 1);
  destroyGameState(state);
}
END_TEST

START_TEST(test_user_input) {
  GameState *state = createGameState();
  state->current_piece = get_tetromino_by_index(0);
  state->current_piece.x = (WIDTH - 4) / 2;
  state->current_piece.y = HEIGHT / 2;

  Tetromino original = state->current_piece;
  userInput(Action, false, state);
  ck_assert_int_ne(state->current_piece.matrix[0][2], original.matrix[0][2]);

  userInput(Pause, false, state);
  ck_assert_int_eq(state->game_status, PAUSED);

  userInput(Terminate, false, state);
  ck_assert_int_eq(state->game_status, GAME_OVER);

  destroyGameState(state);
}
END_TEST

START_TEST(test_action_rotate) {
  GameState *state = createGameState();
  state->current_piece = get_tetromino_by_index(0);
  state->current_piece.x = (WIDTH - 4) / 2;
  state->current_piece.y = HEIGHT / 2;

  action_rotate(state);
  ck_assert_int_eq(state->current_piece.matrix[0][2], 1);

  destroyGameState(state);
}
END_TEST

// Новый тест для userInput (действие Start)
START_TEST(test_userInput_Start) {
  GameState *state = createGameState();
  userInput(Start, false, state);
  ck_assert_int_eq(state->score, 0);
  ck_assert_int_eq(state->level, 1);
  ck_assert_int_eq(state->game_status, PLAYING);
  destroyGameState(state);
}
END_TEST

// Новый тест для userInput (действие Up)
START_TEST(test_userInput_Up) {
  GameState *state = createGameState();
  userInput(Up, false, state);
  // Проверяем, что ничего не изменилось, так как Up не делает ничего
  ck_assert_int_eq(state->game_status, PLAYING);
  destroyGameState(state);
}
END_TEST

// Новый тест для userInput (действие Down)
START_TEST(test_userInput_Down) {
  GameState *state = createGameState();
  state->current_piece = get_tetromino_by_index(0);
  state->current_piece.x = (WIDTH - 4) / 2;
  state->current_piece.y = 0;

  int original_y = state->current_piece.y;
  userInput(Down, false, state);
  ck_assert_int_eq(state->current_piece.y, original_y + 1);
  destroyGameState(state);
}
END_TEST

// Новый тест для userInput (действие Right)
START_TEST(test_userInput_Right) {
  GameState *state = createGameState();
  state->current_piece = get_tetromino_by_index(0);
  state->current_piece.x = (WIDTH - 4) / 2;
  int original_x = state->current_piece.x;
  userInput(Right, false, state);
  ck_assert_int_eq(state->current_piece.x, original_x + 1);
  destroyGameState(state);
}
END_TEST

// Новый тест для check_level (все диапазоны очков)
START_TEST(test_check_level_all_ranges) {
  GameState *state = createGameState();
  state->score = 4800;
  check_level(state);
  ck_assert_int_eq(state->level, 9);

  state->score = 4200;
  check_level(state);
  ck_assert_int_eq(state->level, 8);

  state->score = 3600;
  check_level(state);
  ck_assert_int_eq(state->level, 7);

  destroyGameState(state);
}
END_TEST

// Новый тест для deleting_line (удаление трёх строк)
START_TEST(test_deleting_line_three) {
  GameState *state = createGameState();
  for (int y = HEIGHT - 1; y > HEIGHT - 4; y--) {
    for (int x = 0; x < WIDTH; x++) {
      state->field[y][x] = 1;
    }
  }
  deleting_line(state);
  ck_assert_int_eq(state->score, 700);  // За три строки
  ck_assert_int_eq(state->total_lines_cleared, 3);
  destroyGameState(state);
}
END_TEST

START_TEST(test_action_rotate_wall_kick) {
  GameState *state = createGameState();
  state->current_piece = get_tetromino_by_index(0);  // I-фигура
  state->current_piece.x = 0;                        // На границе
  state->current_piece.y = HEIGHT / 2;

  // int original_x = state->current_piece.x;
  action_rotate(state);
  // Проверяем, что фигура сместилась (wall kick) или осталась в допустимой
  // позиции
  ck_assert_int_ge(state->current_piece.x, 0);
  ck_assert_int_eq(state->current_piece.matrix[0][2], 1);

  destroyGameState(state);
}
END_TEST

Suite *brick_game_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("BrickGame");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_initializing_game);
  tcase_add_test(tc_core, test_get_tetromino_by_index);
  tcase_add_test(tc_core, test_get_random_tetromino);
  tcase_add_test(tc_core, test_rotate_tetromino);
  tcase_add_test(tc_core, test_is_action_valid);
  tcase_add_test(tc_core, test_move_to_left_right);
  tcase_add_test(tc_core, test_action_down);
  tcase_add_test(tc_core, test_copy_free_field);
  tcase_add_test(tc_core, test_copy_free_tetromino);
  tcase_add_test(tc_core, test_check_level);
  tcase_add_test(tc_core, test_generating_new_shape);
  tcase_add_test(tc_core, test_move_down_auto);
  tcase_add_test(tc_core, test_fixing_piece);
  tcase_add_test(tc_core, test_deleting_line);
  tcase_add_test(tc_core, test_load_high_score);
  tcase_add_test(tc_core, test_check_tact_from_level);
  tcase_add_test(tc_core, test_user_input);
  tcase_add_test(tc_core, test_action_rotate);
  tcase_add_test(tc_core, test_userInput_Start);
  tcase_add_test(tc_core, test_userInput_Up);
  tcase_add_test(tc_core, test_userInput_Down);
  tcase_add_test(tc_core, test_userInput_Right);
  tcase_add_test(tc_core, test_check_level_all_ranges);
  tcase_add_test(tc_core, test_deleting_line_three);
  tcase_add_test(tc_core, test_action_rotate_wall_kick);

  suite_add_tcase(s, tc_core);
  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = brick_game_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}