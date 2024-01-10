CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c99 -pedantic -Wvla -g
CPPFLAGS = -Isrc -Isrc/parser -Isrc/lexer -Isrc/ast

OBJS = src/lexer/lexer.o

OBJ_TOKEN = tests/token_printer.o

all: token

token: $(OBJS) $(OBJ_TOKEN)
	$(CC) -g -o eval_token $(OBJS) $(OBJ_TOKEN)
	
$(OBJ_TOKEN):
	$(CC) -g -c tests/token_printer.c -o $(OBJ_TOKEN)

clean:
	$(RM) $(OBJS) $(OBJ_AST) $(OBJ_TOKEN) $(OBJ_RPN) eval_*

