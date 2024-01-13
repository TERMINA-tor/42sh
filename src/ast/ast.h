#ifndef AST_H
#define AST_H

#include <stdlib.h>

enum ast_type
{
    AST_IF,
    AST_ELIF,
    AST_WORD,
    AST_COMMAND
};

struct ast
{
    enum ast_type type; ///< node type
    struct ast **children; ///< list of children
    size_t nbchildren; ///< number of children
    char *value; ///< value of the current token (can be a word or a command)
};

/**
 ** \brief Allocates a node with the given type
 */
struct ast *ast_new(enum ast_type type);

/**
 ** \brief free the given ast
 */

void ast_free(struct ast *ast);

/**
 ** \brief append child to parent->children
 */
int add_child_ast(struct ast *parent, struct ast *child);

#endif