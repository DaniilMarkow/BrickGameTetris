CC = gcc
CFLAGS = -Wall -Wextra -I../../brick_game/tetris/include
LDFLAGS = -lncurses

SRC_DIR = ../../brick_game/tetris
CLI_DIR = .

SOURCES = $(CLI_DIR)/cli.c $(SRC_DIR)/brick_game_tetris.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = tetris

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
