CC=gcc
CFLAGS=-std=c99 -pedantic -Werror -Wall -Wextra -Wvla -fsanitize=address -g
DEPENDENCIES= test_lexer.c lexer.c ../input/input.c ../utils/Dstring/dstring.c

all: test

test:
	$(CC) $(CFLAGS) -D_POSIX_C_SOURCE=200809L -o tests/test $(DEPENDENCIES)

check: test
	python3 -m pytest -vv tests/test_lexer.py
	make clean

clean:
	$(RM) *.o tests/test
