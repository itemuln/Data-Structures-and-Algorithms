CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lncurses -lpthread

SRCS = Main.c common.c temple_run.c geometry_dash.c flappy_bird.c snake.c dino_run.c
OBJS = $(SRCS:.c=.o)
TARGET = arcade_games

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
