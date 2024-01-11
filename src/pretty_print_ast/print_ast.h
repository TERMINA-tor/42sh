#ifndef PRINT_AST_H
#define PRINT_AST_H

#include <stdio.h>

#include "../ast/ast.h"

// Function prototype for little-pretty-printing an AST
void little_print_ast(struct ast *res, int depth);

// Function prototype for pretty-printing an AST
void pretty_print_ast(struct ast *res);

#endif // PRINT_AST_H