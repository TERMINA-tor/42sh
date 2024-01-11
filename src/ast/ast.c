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
	else if (!ast->nbchildren)
		free(ast);
	else
	{
		for (size_t child = 0; child < ast->nbchildren; child++)
		{
			ast_free(ast->children[child]);
		}
		free(ast);
	}
}
