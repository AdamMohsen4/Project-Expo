CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Iinclude -O2
SRCS = src/main.c src/game.c src/world.c src/ui.c src/input.c
OBJS = $(SRCS:src/%.c=build/%.o)
TARGET = switchback

all: $(TARGET)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

build:
	mkdir -p build

clean:
	rm -rf build $(TARGET)

.PHONY: all clean build
