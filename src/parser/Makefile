CC=gcc
CFLAGS=-std=c99 -pedantic -Werror -Wall -Wextra -Wvla -g -fsanitize=address

.PHONY: all clean

all: main

DEPENDENCIES = main.c ../input/input.c ../lexer/lexer.c ../ast/ast.c parser.c

main:
	$(CC) $(CFLAGS) -D_POSIX_C_SOURCE=200809L -o 42sh $(DEPENDENCIES)

check: main
	pytest

clean:
	$(RM) *.o 42sh
