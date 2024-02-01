CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -Wvla -o1 -g -fsanitize=address

DEP = src/parser/parser_compound_list.c src/parser/parser_input.c src/parser/parser_rule_shell.c src/parser/parser_loops.c src/ast/ast.c src/input/input.c src/lexer/lexer.c src/utils/Dstring/dstring.c src/main.c src/expansion/expansion.c src/eval/basic_builtins.c src/eval/evaluation_process.c src/eval/eval_pipe_negation.c src/eval/eval_redirections.c src/eval/cd_builtin.c src/eval/advanced_builtins.c

all: test

test: 
	$(CC) $(CFLAGS) -D_POSIX_C_SOURCE=200809L -o src/42sh $(DEP)

check:  test
	python3 -m pytest tests/test_suite.py
	make clean

clean:
	$(RM) *.o src/42sh
