#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../ast/ast.h"
#include "builtins.h"
#include "evaluation_process.h"

int string_to_int(char *string)
{
    int i = 0;
    int r = 0;
    while (string[i] != 0)
    {
        r *= 10;
        r += string[i];
        i++;
    }
    return r;
}
int builtin_exit(struct ast_cmd *cmd)
{
    int exit_status = 0; // Default exit status
    if (cmd->num_words > 1)
    {
        exit_status = atoi(cmd->words[1]); // Convert the argument to an integer
    }

    clean_ast();
    // free lexer
    //  close file
    exit(exit_status); // Exit the program with the provided status
    return 0;
}

int builtin_export(struct ast_cmd *cmd)
{
    for (size_t i = 1; i < cmd->num_words; ++i)
    {
        char *var = cmd->words[i];
        char *value = strchr(var, '=');

        if (value != NULL)
        {
            *value = '\0'; // Split the string into two strings: var and value
            value++;
            setenv(var, value, 1); // Set the environment variable
        }
        else
        {
            // variable is already in the environment
        }
    }
    return 0;
}

int builtin_unset(struct ast_cmd *cmd)
{
    for (size_t i = 1; i < cmd->num_words; ++i)
    {
        unsetenv(cmd->words[i]); // Remove the variable from the environment
    }
    return 0;
}

int builtin_continue(struct ast_cmd *cmd, int inside_loop)
{
    if (!inside_loop)
    {
        fprintf(stderr, "continue: should be inside a loop\n");
        return 1;
    }

    if (cmd->num_words > 2)
    {
        fprintf(stderr, "continue: too much args\n");
        return 1;
    }
    int i = 0;
    if (cmd->num_words == 2)
    {
        i = string_to_int(cmd->words[1]);
    }
    // If inside a loop, signal to continue with the next iteration
    set_loop_continue_flag(i);
    return 0;
}

int builtin_break(struct ast_cmd *cmd, int inside_loop)
{
    if (!inside_loop)
    {
        fprintf(stderr, "break: should be inside a loop\n");
        return 1;
    }

    if (cmd->num_words > 2)
    {
        fprintf(stderr, "break: too much args\n");
        return 1;
    }
    int i = 0;
    if (cmd->num_words == 2)
    {
        i = string_to_int(cmd->words[1]);
    }
    // If inside a loop, signal to break out of the loop
    set_loop_break_flag(i);
    return 0;
}
