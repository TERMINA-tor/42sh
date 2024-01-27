CC=gcc
CFLAGS=-std=c99 -pedantic -Werror -Wall -Wextra -Wvla -o1 -g -fsanitize=address

DEP = parser/parser_compound_list.c parser/parser_input.c parser/parser_rule_shell.c parser/parser_loops.c ast/ast.c input/input.c lexer/lexer.c utils/Dstring/dstring.c main.c expansion/expansion.c

all: test

test: 
	$(CC) $(CFLAGS) -D_POSIX_C_SOURCE=200809L -o test $(DEP)

clean:
	$(RM) *.o test
