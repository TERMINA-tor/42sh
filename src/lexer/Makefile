CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c99 -pedantic -Wvla -g -fsanitize=address
CPPFLAGS = -Isrc -Isrc/parser -Isrc/lexer -Isrc/ast

OBJS = lexer.o

OBJ_TOKEN = tests/token_printer.o
OBJ_PEEK = tests/peek_printer.o

all: token

token: $(OBJS) $(OBJ_TOKEN)
	$(CC) $(CFLAGS) -o eval_token $(OBJS) $(OBJ_TOKEN)

peek: $(OBJS) $(OBJ_PEEK)
	$(CC) $(CFLAGS) -o eval_peek $(OBJS) $(OBJ_PEEK)

check: token
	python3 -m pytest -vv tests/test_lexer.py
	make clean

check_peek: peek
	python3 -m pytest -vv tests/test_lexer_peek.py
	make clean

$(OBJ_TOKEN):
	$(CC) $(CFLAGS) -c tests/token_printer.c -o $(OBJ_TOKEN)

$(OBJ_PEEK):
	$(CC) $(CFLAGS) -c tests/peek_printer.c -o $(OBJ_PEEK)

clean:
	$(RM) $(OBJS) $(OBJ_TOKEN) eval_* $(OBJ_PEEK)

