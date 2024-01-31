#include "eval_pipe_negation.h"

int execute_command(struct ast_cmd *node);

int eval_pipeline(struct ast *node)
{
    struct ast_pipeline *pipeline = (struct ast_pipeline *)node;
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }

    close(pipefd[0]);
    dup2(pipefd[1], 1);
    close(pipefd[1]);
    int ret = execute_command((struct ast_cmd *)pipeline->left_cmd);
    if (ret != 0)
        return ret;

    close(pipefd[1]);
    dup2(pipefd[0], 0);
    close(pipefd[0]);
    return execute_command((struct ast_cmd *)pipeline->right_cmd);
}
