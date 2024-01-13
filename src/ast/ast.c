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
	if (! ast)
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
