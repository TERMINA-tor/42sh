CC=gcc
CFLAGS=-std=c99 -pedantic -Werror -Wall -Wextra -Wvla -fsanitize=address -g
DEPENDENCIES=parser.c ../lexer/lexer.c ../input/input.c

all: test

test:
	$(CC) $(CFLAGS) -D_POSIX_C_SOURCE=200809L -o test $(DEPENDENCIES) tests/main.c

check: test
	python3 -m pytest -vv tests/parser_test.py
	make clean

clean:
	$(RM) *.o test
