CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c99 -pedantic -Wvla -g
CPPFLAGS = -Isrc -Isrc/parser -Isrc/lexer -Isrc/ast

OBJS = basic_builtins.o

OBJ = test/builtins_test.c

all: builtins

builtins: $(OBJS) $(OBJ_BUILTINS)
	$(CC) -g -fsanitize=address -o builtins $(OBJS) $(OBJ) $(OBJ_BUILTINS)

check: builtins
	./builtins
	make clean

$(OBJ_BUILTINS):
	$(CC) -g -fsanitize=address -c $(OBJ) -o $(OBJ_BUILTINS)

clean:
	$(RM) $(OBJS) $(OBJ_BUILTINS) builtins

