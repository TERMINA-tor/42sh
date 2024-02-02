#include "eval_redirections.h"

static int eval_redir_input(struct ast_redirection *redirection, int i);
static int eval_redir_output(struct ast_redirection *redirection, int i);
static int eval_append_output(struct ast_redirection *redirection, int i);
static int eval_ampredir_output(struct ast_redirection *redirection, int i);
static int eval_ampredir_input(struct ast_redirection *redirection, int i);
static int eval_force_output_redir(struct ast_redirection *redirection, int i);
static int eval_redir_input_output(struct ast_redirection *redirection, int i);

int execute_command(struct ast_cmd *command_node);

int eval_redirections(struct ast *node)
{
    struct ast_redirection *redirection = (struct ast_redirection *)node;
    int ret = 0;
    for (int i = 0; i < redirection->num_filenames; i++)
    {
        switch (redirection->type)
        {
        case TOKEN_REDIRECT_INPUT:
            ret = eval_redir_input(redirection, i);
            break;
        case TOKEN_REDIRECT_OUTPUT:
            ret = eval_redir_output(redirection, i);
            break;
        case TOKEN_APPEND_OUTPUT:
            ret = eval_append_output(redirection, i);
            break;
        case TOKEN_AMPREDIR_OUTPUT:
            ret = eval_ampredir_output(redirection, i);
            break;
        case TOKEN_AMPREDIR_INPUT:
            ret = eval_ampredir_input(redirection, i);
            break;
        case TOKEN_FORCE_OUTPUT_REDIR:
            ret = eval_force_output_redir(redirection, i);
            break;
        case TOKEN_REDIRECT_INPUT_OUTPUT:
            ret = eval_redir_input_output(redirection, i);
            break;
        default:
            break;
        }
        if (ret != 0)
            return ret; // If there was an error, return immediately
    }
    return ret;
}

static int eval_redir_input(struct ast_redirection *redirection, int i)
{
    int fd;
    int ret = 0;
    fd = open(redirection->filenames[i], O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }
    ret = dup2(fd, 0);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    if (redirection->num_filenames - 1 == i)
        ret = execute_command((struct ast_cmd *)redirection->command);
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return ret;
}

static int eval_redir_output(struct ast_redirection *redirection, int i)
{
    int fd;
    int ret = 0;
    fd = open(redirection->filenames[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }
    dup2(fd, 1);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    if (redirection->num_filenames - 1 == i)
        if (redirection->num_filenames - 1 == i)
            ret = execute_command((struct ast_cmd *)redirection->command);
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return ret;
}

static int eval_append_output(struct ast_redirection *redirection, int i)
{
    int fd;
    int ret = 0;
    fd = open(redirection->filenames[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }
    dup2(fd, 1);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    if (redirection->num_filenames - 1 == i)
        ret = execute_command((struct ast_cmd *)redirection->command);
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return ret;
}

static int eval_ampredir_output(struct ast_redirection *redirection, int i)
{
    int fd;
    int ret = 0;
    fd = open(redirection->filenames[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }
    dup2(fd, 1);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    dup2(1, 2);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    if (redirection->num_filenames - 1 == i)
        ret = execute_command((struct ast_cmd *)redirection->command);
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return ret;
}

static int eval_ampredir_input(struct ast_redirection *redirection, int i)
{
    int fd;
    int ret = 0;
    fd = open(redirection->filenames[i], O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }
    dup2(fd, 0);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    dup2(0, 1);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    if (redirection->num_filenames - 1 == i)
        ret = execute_command((struct ast_cmd *)redirection->command);
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return ret;
}

static int eval_force_output_redir(struct ast_redirection *redirection, int i)
{
    int fd;
    int ret = 0;
    fd = open(redirection->filenames[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }
    dup2(fd, 1);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    dup2(1, 2);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    if (redirection->num_filenames - 1 == i)
        ret = execute_command((struct ast_cmd *)redirection->command);
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return ret;
}

static int eval_redir_input_output(struct ast_redirection *redirection, int i)
{
    int fd;
    int ret = 0;
    fd = open(redirection->filenames[i], O_RDWR | O_CREAT, 0644);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }
    dup2(fd, 0);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    dup2(fd, 1);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    if (redirection->num_filenames - 1 == i)
        ret = execute_command((struct ast_cmd *)redirection->command);
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return ret;
}
