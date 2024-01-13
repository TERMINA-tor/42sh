#include <stdio.h>

#include "parser.h"

#include "../input/input.h"

struct queue
{
    struct queue *next;
    struct queue *previous;
    struct queue *last;
    struct ast *value;
};


struct queue *Queue(void)
{
    struct queue *q = malloc(sizeof(struct queue));
    q->next = NULL;
    q->last = NULL;
    q->previous = NULL;
    return q;
}

void enqueue(struct queue *q, struct ast *x)
{
    struct queue *new = Queue();
    new->next = NULL;
    new->previous = NULL;
    new->value = x;

    if (q->next == NULL)
    {
        q->next = new;
        q->last = new;
        new->previous = q;
    }
    else
    {
        new->previous = q->last;
        q->last->next = new;
        q->last = new;
        new->last = q->last;
    }
}

struct ast *dequeue(struct queue *q)
{
    if (q->next != NULL)
    {
        struct queue *tmp = q->next;
        q->next = q->next->next;
        struct ast *t = tmp->value;
        free(tmp);
        if (q->next == q)
        {
            q->next = NULL;
            q->last = NULL;
        }
        return t;
    }
    return NULL;
}

void free_queue(struct queue *q)
{
   if (!q)
	   return;
   while (q->next)
   {
	   free(q->next);
   } 
   if (q)
	   free(q);
}

int q_isempty(struct queue *q)
{
    return q->next == NULL;
}

char *whatis(struct ast *ast)
{
	switch (ast->type)
	{
	case AST_IF:
		return "if";
		break;
	case AST_ELIF:
		return "elif";
		break;
	default:
		return ast->value;
		break;
	}
	return "";
}

void print_ast(struct ast *a)
{
	struct ast *flag = ast_new(AST_WORD);
	flag->value = "flag";
	struct queue *q = Queue();
	enqueue(q, a);
	enqueue(q, flag);
	int nbtoken = 0;
	printf("digraph G {\n");
	while (! q_isempty(q))
	{
		char curr[20] = {0};
		struct ast *b = dequeue(q);
		if (!strcmp(b->value, "flag"))
		{
			if (! q_isempty(q))
				enqueue(q, flag);
			continue;
		}
		switch (b->type)
		{
		case AST_IF:
			sprintf(curr, "if%d", nbtoken);
			break;
		case AST_ELIF:
			sprintf(curr, "elif%d", nbtoken);
			break;
		case AST_WORD:
			sprintf(curr, "{word%d: %s}", nbtoken, b->value);
			break;
		case AST_COMMAND:
			sprintf(curr, "{cmd%d: %s}", nbtoken, b->value);
			break;
		default:
			break;
		}
		for (size_t i = 0; i  < b->nbchildren; i++)
		{
			printf("\t%s -> %s\n", curr, whatis(b->children[i]));
			nbtoken++;
			enqueue(q, b->children[i]);
		}
	}
	printf("}\n");
	free(flag);
	free_queue(q);
}

int main(int argc, char **argv)
{
	char *input = get_input(argc, argv);
	struct lexer *lexer = lexer_new(input);
	struct ast *res = NULL;
	enum parser_status result = parse_input(&res, lexer);
	
	//print_ast(res);
	free(input);
	lexer_free(lexer);

	if (!res)
		return 1;
	ast_free(res);
	return result;
}
