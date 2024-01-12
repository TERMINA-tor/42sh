CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c99 -pedantic -Wvla -g
CPPFLAGS = -Isrc -Isrc/parser -Isrc/lexer -Isrc/ast

OBJS = basic_builtins.o

OBJ = test/main.c

all: builtins

builtins: $(OBJS) $(OBJ_BUILTINS)
	$(CC) $(CFLAGS) -g -fsanitize=address -o builtin_echo $(OBJS) $(OBJ) $(OBJ_BUILTINS)

check: builtins
	python3 -m pytest -vv test/test_builtins.py
	make clean

$(OBJ_BUILTINS):
	$(CC) $(CFLAGS) -g -fsanitize=address -c $(OBJ) -o $(OBJ_BUILTINS)

clean:
	$(RM) $(OBJS) $(OBJ_BUILTINS) builtin_echo

