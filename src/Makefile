CC = gcc
OS = $(shell uname)
CFLAGS = -Wall -Werror -Wextra -g -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LDFLAGS = -lncurses
PREFIX ?= /usr/local
BUILD_DIR = build
EXEC = $(BUILD_DIR)/tetris
TEST_EXEC = $(BUILD_DIR)/test_tetris
GAME_SRC = brick_game/tetris/brick_game_tetris.c
CLI_SRC = gui/cli/cli.c
TEST_SRC = brick_game/tests/test_brick_game_tetris.c
GAME_OBJ = $(BUILD_DIR)/brick_game_tetris.o
CLI_OBJ = $(BUILD_DIR)/cli.o
TEST_OBJ = $(BUILD_DIR)/test_brick_game_tetris.o
OBJ = $(GAME_OBJ) $(CLI_OBJ)

ifeq ($(OS), Darwin)
    TEST_LDFLAGS = -lm -lcheck
    OPEN_CMD = open
else
    TEST_LDFLAGS = -lcheck -lcheck_pic -pthread -lrt -lm -lsubunit
    OPEN_CMD = xdg-open
endif

.PHONY: all clean install uninstall test gcov_report dvi dist style test_style test_valgrind test_cppcheck

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

$(GAME_OBJ): $(GAME_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLI_OBJ): $(CLI_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_EXEC)
	./$(TEST_EXEC)

$(TEST_EXEC): $(TEST_OBJ) $(BUILD_DIR)/brick_game_tetris_gcov.o | $(BUILD_DIR)
	$(CC) $(TEST_OBJ) $(BUILD_DIR)/brick_game_tetris_gcov.o -o $@ $(TEST_LDFLAGS) --coverage

$(TEST_OBJ): $(TEST_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $< -o $@

$(BUILD_DIR)/brick_game_tetris_gcov.o: $(GAME_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $< -o $@

gcov_report: test
	mkdir -p gcov_report
	lcov --capture --directory . --output-file gcov_report/test.info --no-external --exclude 'tests/*' --exclude 'brick_game/tests/*' --ignore-errors unused
	genhtml gcov_report/test.info --output-directory gcov_report
	$(OPEN_CMD) gcov_report/index.html

clean:
	rm -rf $(BUILD_DIR) *.o *.a *.gcda *.gcno *.gcov coverage.info gcov_report logs s21_test_matrix.dSYM test.info
	rm -f ./cli.o ../../brick_game/tetris/brick_game_tetris.o tetris

install: $(EXEC)
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(EXEC) $(DESTDIR)$(PREFIX)/bin/tetris

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/tetris

dvi:
	@echo "No documentation to build"

dist:
	@echo "No distribution to create"

style:
	@clang-format -i -style=google brick_game/tetris/*.c brick_game/tetris/include/*.h gui/cli/*.c brick_game/tests/*.c

test_style:
	@clang-format -n -style=google brick_game/tetris/*.c brick_game/tetris/include/*.h gui/cli/*.c brick_game/tests/*.c

test_valgrind: test
	rm -rf logs
	mkdir logs
	touch logs/logs.txt
	export CK_FORK=no && \
		valgrind --tool=memcheck --leak-check=yes ./$(TEST_EXEC) > logs/logs.txt 2>&1

test_cppcheck: test
	@cppcheck --enable=all --suppress=missingIncludeSystem \
		brick_game/tetris/*.c gui/cli/*.c brick_game/tests/*.c

$(BUILD_DIR):
	mkdir -p $@