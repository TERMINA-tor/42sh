#include "../ast/ast.h"
#include "builtins.h"
#include "stdio.h"
#include "string.h"

int execute_command(struct ast *command_node);
char **convert_children_to_argv(struct ast *node);
int evaluate_if(struct ast *if_node);

int evaluate_node(struct ast *node)
{
    if (node == NULL)
    {
        return -1;
    }

    switch (node->type)
    {
    case AST_IF:
        return evaluate_if(node);
    case AST_COMMAND:
        return execute_command(node);
    case AST_WORD:
        fprintf(stderr, "Unexpected word: %s\n", node->value);
        return -1;
    default:
        fprintf(stderr, "Unknown AST node type\n");
        return -1;
    }
}

int evaluate_if(struct ast *if_node)
{
    if (if_node == NULL || if_node->nbchildren == 0)
   	goto error; 

    // Evaluate the condition
    int condition_result = evaluate_node(if_node->children[0]);
    if (condition_result == -1)
    	goto error;
    
    if (condition_result == builtin_true())
    {
        // If condition is true and there is a 'true' block
        if (if_node->nbchildren > 1)
        {
            return evaluate_node(if_node->children[1]);
        }
	return -1;
    }
    else
    {
        // If condition is false and there is an 'else' block
        if (if_node->nbchildren > 2)
        {
            return evaluate_node(if_node->children[2]);
        }
    }

    return 0;

error:
    fprintf(stderr, "./42sh: error near \'if\'");
    return -1;
}

int execute_command(struct ast *command_node)
{
    if (strcmp(command_node->value, "echo") == 0)
    {
        char **argv = convert_children_to_argv(command_node);
        int res = builtin_echo(argv);
        free(argv);
        return res;
    }
    else if (strcmp(command_node->value, "true") == 0)
    {
        return builtin_true();
    }
    else if (strcmp(command_node->value, "false") == 0)
    {
        return builtin_false();
    }
    else
    {
        fprintf(stderr, "Unknown command: %s\n", command_node->value);
        return -1;
    }
}

char **convert_children_to_argv(struct ast *node)
{
    char **argv = malloc((node->nbchildren + 2) * sizeof(char *));
    if (!argv)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    argv[0] = node->value; // Command name
    for (size_t i = 0; i < node->nbchildren; i++)
    {
        argv[i + 1] = node->children[i]->value;
    }
    argv[node->nbchildren + 1] = NULL; // Null-terminate the argv array

    return argv;
}
