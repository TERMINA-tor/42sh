#include "eval_pipe_negation.h"

int execute_command(struct ast_cmd *node);

int eval_pipeline(struct ast *node)
{
    struct ast_pipeline *pipeline = (struct ast_pipeline *)node;
    int pipefd[2];
    int ret = 0;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        ret = execute_command((struct ast_cmd *)pipeline->left_cmd);
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
        ret = execute_command((struct ast_cmd *)pipeline->right_cmd);
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
    wait(NULL);

    return ret;
}

// int eval_negation(struct ast *node)
// {
//     struct ast_negation *negation = (struct ast_negation *)node;
//     int res = evaluate_node(negation->child);
//     if (res == builtin_true())
//     {
//         return builtin_false();
//     }
//     else if (res == builtin_false())
//     {
//         return builtin_true();
//     }
//     return res;
// }