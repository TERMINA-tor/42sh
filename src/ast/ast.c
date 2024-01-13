#include "ast.h"

struct ast *ast_new(enum ast_type type)
{
    struct ast *new = calloc(1, sizeof(struct ast));

    if (!new)
        return NULL;

    new->type = type;
    return new;
}

void ast_free(struct ast *ast)
{
    if (!ast)
        return;
    if (ast->value)
        free(ast->value);
    for (size_t i = 0; i < ast->nbchildren; i++)
    {
        ast_free(ast->children[i]);
    }
    if (ast->nbchildren)
        free(ast->children);
    free(ast);
}

int add_child_ast(struct ast *parent, struct ast *child)
{
    parent->nbchildren += 1;
    size_t new_size = parent->nbchildren * sizeof(struct ast);
    parent->children = realloc(parent->children, new_size);
    if (parent->children)
    {
        parent->children[parent->nbchildren - 1] = child;
        return 0;
    }
    return 1;
}
