#include "evaluation_process.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../ast/ast.h"
#include "builtins.h"

int evaluate_node(struct ast *node);
int execute_command(struct ast_cmd *command_node);
char **convert_children_to_argv(struct ast *node);
int evaluate_if(struct ast_if *if_node);
int execute_until(struct ast_loop *until_node);
int execute_while(struct ast_loop *while_node);
int execute_command_non_builtin(char *argv[], size_t num_words);
int inside_loop = 0;
int break_loop = 0;

int evaluate_ast(struct ast_sequence *node)
{
    for (size_t i = 0; i < node->num_commands; i++)
    {
        int res = evaluate_node(*(node->commands + i));
        if (res != builtin_true())
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
        break;
    case AST_COMMAND:
        return execute_command((struct ast_cmd *)node);
        break;
    case AST_UNTIL:
        return execute_until((struct ast_loop *)node);
        break;
    case AST_WHILE:
        return execute_while((struct ast_loop *)node);
        break;
    case AST_SEQUENCE:
        return evaluate_ast((struct ast_sequence *)node);
    default:
        fprintf(stderr, "Unknown AST node type\n");
        return -1;
        break;
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

static int is_reserved_word(char *s)
{
    const char *reserved_words[] = { "",     "\n",   "if",    "else",  "then",
                                     "elif", "fi",   "while", "until", "for",
                                     "do",   "done", "&&",    "||",    "|",
                                     ";",    "<",    ">",     ">>",    ">&",
                                     "<&",   ">|" };
    size_t len = sizeof(reserved_words) / sizeof(char *);
    for (size_t i = 0; i < len; i++)
    {
        if (!strcmp(s, reserved_words[i]))
            return 1;
    }
    return 0;
}

void set_loop_break_flag();

int execute_command(struct ast_cmd *command_node)
{
    struct ast_cmd *dupe = (struct ast_cmd *)command_node;
    for (size_t i = 0; i < dupe->num_words; i++) // TODO
    {
        char *old_wd = dupe->words[i];
        dupe->words[i] = expand(old_wd);
        free(old_wd);
    }
    if (strcmp(*command_node->words, "echo") == 0)
    {
        return builtin_echo(command_node->words + 1, command_node->num_words);
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
        if (command_node->num_words && is_reserved_word(command_node->words[0]))
        {
            fprintf(stderr, "./42sh: %s is not a command.\n",
                    command_node->words[0]);
            return 2;
        }
        int status = execute_command_non_builtin(command_node->words,
                                                 command_node->num_words);
        if (status == -1)
        {
            fprintf(stderr, "Unknown command: %s\n", *command_node->words);
        }
        else
        {
            return status;
        }
        return -1;
    }
}

void set_loop_break_flag()
{
    break_loop = 1;
}

int execute_command_non_builtin(char *argv[], size_t num_words)
{
    if (num_words == 0 || argv == NULL || argv[0] == NULL)
    {
        fprintf(stderr, "Invalid command.\n");
        return -1;
    }
    pid_t pid = fork(); // Create a new process

    if (pid == -1)
    {
        // If fork() returns -1, an error occurred
        perror("fork failed");
        return -1;
    }
    else if (pid == 0)
    {
        // Child process

        // Ensure argv is NULL-terminated for execvp
        char **exec_argv = malloc((num_words + 1) * sizeof(char *));
        if (exec_argv == NULL)
        {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < num_words; i++)
        {
            exec_argv[i] = argv[i];
        }
        exec_argv[num_words] = NULL; // Null-terminate the array

        execvp(exec_argv[0],
               exec_argv); // Replace the child process with the new program
        // If execvp returns, it must have failed.
        perror("execvp failed");
        free(exec_argv); // Clean up the allocated memory
        exit(127); // Terminate child process
    }
    else
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for the child to complete
        if (WIFEXITED(status))
        {
            // Return child's exit status if it exited normally
            return WEXITSTATUS(status);
        }
        else
        {
            // Child did not exit normally
            return -1;
        }
    }
}

int execute_until(struct ast_loop *until_node)
{
    while (!evaluate_node(until_node->condition) && !break_loop)
    {
        evaluate_node(until_node->then_body);
    }
    if (break_loop)
    {
        break_loop = 0;
    }

    return builtin_true();
}

int execute_while(struct ast_loop *while_node)
{
    while (evaluate_node(while_node->condition) && !break_loop)
    {
        evaluate_node(while_node->then_body);
    }
    if (break_loop)
    {
        break_loop = 0;
    }

    return builtin_true();
}
