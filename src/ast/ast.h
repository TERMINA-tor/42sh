#ifndef AST_H
#define AST_H

#include <unistd.h>

enum ast_type
{
    AST_IF,
    AST_ELSE,
    AST_ELIF,
    AST_THEN,
    AST_WORD,
    AST_COMMAND
};

struct ast
{
    enum ast_type type; // ast type
    struct ast **children; // list of children
    size_t nbchildren; // number of children
};      

/**
 ** \brief Allocate a new ast with the given type
 */
struct ast *ast_new(enum ast_type type);

/**
 ** \brief Recursively free the given ast
 */
void ast_free(struct ast *ast);

#endif /* !AST_H */
