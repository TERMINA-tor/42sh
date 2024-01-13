#include "../ast/ast.h"
#include "builtins.h"
#include "stdio.h"
#include "string.h"

int execute_command(struct ast *command_node);
char **convert_children_to_argv(struct ast *node);

int evaluate_ast(struct ast *node)
{
    if (node == NULL)
    {
        return -1;
    }

    switch (node->type)
    {
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