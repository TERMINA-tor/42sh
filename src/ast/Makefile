CC=gcc
CFLAGS=-std=c99 -pedantic  -Werror -Wall -Wextra -Wvla -g -fsanitize=address

all: test

test:
	$(CC) $(CFLAGS) -o test main.c ast.c

clean:
	$(RM) test
