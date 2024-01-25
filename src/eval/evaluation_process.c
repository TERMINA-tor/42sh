#include <stdio.h>
#include <string.h>

#include "../ast/ast.h"
#include "builtins.h"

int evaluate_node(struct ast *node);
int execute_command(struct ast_cmd *command_node);
char **convert_children_to_argv(struct ast *node);
int evaluate_if(struct ast_if *if_node);
int execute_until(struct ast_loop *until_node);

int evaluate_ast(struct ast_sequence *node)
{
    for (size_t i = 0; i < node->num_commands; i++)
    {
        int res = evaluate_node(*(node->commands + i));
        if (res == -1)
        {
            return res;
        }
    }

    return builtin_true();
}
int evaluate_node(struct ast *node)
{
    if (node == NULL)
    {
        return -1;
    }

    switch (node->type)
    {
    case AST_IF:
        return evaluate_if((struct ast_if *)node);
    case AST_COMMAND:
        return execute_command((struct ast_cmd *)node);
    case AST_UNTIL:
        return execute_until((struct ast_loop *)node);
    default:
        fprintf(stderr, "Unknown AST node type\n");
        return -1;
    }
}

int evaluate_if(struct ast_if *if_node)
{
    if (if_node->condition == NULL || if_node->then_body == NULL)
    {
        fprintf(stderr, "Invalid if statement\n");
        return -1;
    }

    // Evaluate the condition
    int condition_result = evaluate_node(if_node->condition);
    if (condition_result == -1)
    {
        fprintf(stderr, "Error evaluating if condition\n");
        return -1;
    }

    if (condition_result == builtin_true())
    {
        // If condition is true and there is a 'true' block
        return evaluate_node(if_node->then_body);
    }
    else
    {
        // If condition is false and there is an 'else' block
        if (if_node->else_body != NULL)
        {
            return evaluate_node(if_node->else_body);
        }
    }

    return 0;
}

int execute_command(struct ast_cmd *command_node)
{
    if (strcmp(*command_node->words, "echo") == 0)
    {
        return builtin_echo(command_node->words + 1);
    }
    else if (strcmp(*command_node->words, "true") == 0)
    {
        return builtin_true();
    }
    else if (strcmp(*command_node->words, "false") == 0)
    {
        return builtin_false();
    }
    else
    {
        fprintf(stderr, "Unknown command: %s\n", *command_node->words);
        return -1;
    }
}

int execute_until(struct ast_loop *until_node)
{
    while (!evaluate_node(until_node->condition))
    {
        evaluate_node(until_node->then_body);
    }

    return builtin_true();
}

// char **convert_children_to_argv(struct ast *node)
// {
//     char **argv = malloc((node->nbchildren + 2) * sizeof(char *));
//     if (!argv)
//     {
//         perror("malloc failed");
//         exit(EXIT_FAILURE);
//     }
//
//     argv[0] = node->value; // Command name
//     for (size_t i = 0; i < node->nbchildren; i++)
//     {
//         argv[i + 1] = node->children[i]->value;
//     }
//     argv[node->nbchildren + 1] = NULL; // Null-terminate the argv array
//
//     return argv;
// }
