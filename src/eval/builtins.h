#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdio.h>

#include "../ast/ast.h"
#include "../utils/Dstring/dstring.h"

// each of the following are built-in functions

int builtin_true(void);

int builtin_false(void);

int builtin_echo(char **argv, size_t nbr_args);

int builtin_exit(struct ast_cmd *cmd);

int builtin_cd(char **args, int nb_args);

int builtin_export(struct ast_cmd *cmd);

// int builtin_dot(struct ast_cmd *cmd);

int builtin_unset(struct ast_cmd *cmd);

int builtin_continue(int inside_loop);

int builtin_break(int inside_loop);

#endif // BUILTINS_H
