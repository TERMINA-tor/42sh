CC=gcc
CFLAGS=-std=c99 -pedantic -Werror -Wall -Wextra -Wvla -g -fsanitize=address

.PHONY: all clean

all: main

DEPENDENCIES = ./src/main.c ./src/input/input.c ./src/lexer/lexer.c ./src/ast/ast.c ./src/parser/parser.c ./src/eval/evaluation_process.c ./src/eval/basic_builtins.c

main:
	$(CC) $(CFLAGS) -D_POSIX_C_SOURCE=200809L -o 42sh $(DEPENDENCIES)

check: main
	python3 -m pytest -vv tests/testsuite.py
	make clean

clean:
	$(RM) *.o 42sh
