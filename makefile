CC=gcc
CFLAGS=-Iinclude -Itomlc99 -lncurses

SRC=main.c src/*.c tomlc99/toml.c

all:
	$(CC) $(SRC) $(CFLAGS) -o patrikos