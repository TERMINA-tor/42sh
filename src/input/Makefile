CC=gcc
CFLAGS=-std=c99 -pedantic -Werror -Wall -Wextra -Wvla -g

.PHONY: all clean

all: main

main:
	$(CC) $(CFLAGS) -D_POSIX_C_SOURCE=200809L -o 42sh main.c input.c

clean:
	$(RM) *.o 42sh
