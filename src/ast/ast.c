#include "ast.h"

#include <stdlib.h>

struct ast *ast_new(enum ast_type type)
{
    struct ast *new = calloc(1, sizeof(struct ast));
    if (!new)
        return NULL;
    new->type = type;
    new->children = NULL;
    new->nbchildren = 0;
    return new;
}

void ast_free(struct ast *ast)
{
    if (ast == NULL)
        return;

    for (size_t i = 0; i < ast->nbchildren; i++)
    {
        ast_free(ast->children[i]);
    }
    free(ast->children);
    free(ast);
}