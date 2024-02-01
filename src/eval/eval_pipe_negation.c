#include "eval_pipe_negation.h"

int execute_command(struct ast_cmd *node);

int eval_pipeline(struct ast *node)
{
    struct ast_pipeline *pipeline = (struct ast_pipeline *)node;
    int pipefd[2];
    int status;
    pid_t pid;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }

    pid = fork();
    if (pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        if (pipeline->left_cmd->type == AST_PIPELINE)
            eval_pipeline(pipeline->left_cmd);
        else
            execute_command((struct ast_cmd *)pipeline->left_cmd);
        exit(EXIT_SUCCESS);
    }
    else if (pid < 0)
    {
        perror("fork");
        return -1;
    }

    pid = fork();
    if (pid == 0)
    {
        close(pipefd[1]);
        dup2(pipefd[0], 0);
        execute_command((struct ast_cmd *)pipeline->right_cmd);
        exit(EXIT_SUCCESS);
    }
    else if (pid < 0)
    {
        perror("fork");
        return -1;
    }

    close(pipefd[0]);
    close(pipefd[1]);

    wait(NULL);
    waitpid(pid, &status, 0);

    if (WIFEXITED(status))
    {
        return WEXITSTATUS(status);
    }
    else
    {
        return -1;
    }
}

int eval_not(struct ast *node)
{
    struct ast_not *not_node = (struct ast_not *)node;
    int status = execute_command((struct ast_cmd *)not_node->command);
    return !status;
}
