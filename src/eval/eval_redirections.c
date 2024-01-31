#include "eval_redirections.h"

static int eval_redir_input(struct ast_redirection *redirection);
static int eval_redir_output(struct ast_redirection *redirection);
static int eval_append_output(struct ast_redirection *redirection);
static int eval_ampredir_output(struct ast_redirection *redirection);
static int eval_ampredir_input(struct ast_redirection *redirection);
static int eval_force_output_redir(struct ast_redirection *redirection);
static int eval_redir_output_input(struct ast_redirection *redirection);

int eval_redirections(struct ast *node)
{
    struct ast_redirection *redirection = (struct ast_redirection *)node;
    int ret = 0;
    switch (redirection->type)
    {
    case TOKEN_REDIRECT_INPUT:
        ret = eval_redir_input(redirection);
        break;
    case TOKEN_REDIRECT_OUTPUT:
        ret = eval_redir_output(redirection);
        break;
    case TOKEN_APPEND_OUTPUT:
        ret = eval_append_output(redirection);
        break;
    case TOKEN_AMPREDIR_OUTPUT:
        ret = eval_ampredir_output(redirection);
        break;
    case TOKEN_AMPREDIR_INPUT:
        ret = eval_ampredir_input(redirection);
        break;
    case TOKEN_FORCE_OUTPUT_REDIR:
        ret = eval_force_output_redir(redirection);
        break;
    case TOKEN_REDIRECT_OUTPUT_INPUT:
        ret = eval_redir_output_input(redirection);
        break;
    default:
        break;
    }
    return ret;
}

static int eval_redir_input(struct ast_redirection *redirection)
{
    int fd;
    int ret = 0;
    fd = open(redirection->filename, O_RDONLY);
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
    ret = evaluate_node(redirection->command);
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return ret;
}

static int eval_redir_output(struct ast_redirection *redirection)
{
    int fd = 1;
    int ret = 0;
    fd = open(redirection->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }
    ret = dup2(fd, 1);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    ret = evaluate_node(redirection->command);
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return ret;
}

static int eval_append_output(struct ast_redirection *redirection)
{
    int fd;
    int ret = 0;
    fd = open(redirection->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }
    ret = dup2(fd, 1);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    ret = evaluate_node(redirection->command);
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return ret;
}

static int eval_ampredir_output(struct ast_redirection *redirection)
{
    int fd;
    int ret = 0;
    fd = open(redirection->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }
    ret = dup2(fd, 1);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    ret = dup2(fd, 2);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    ret = evaluate_node(redirection->command);
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return ret;
}

static int eval_ampredir_input(struct ast_redirection *redirection)
{
    int fd;
    int ret = 0;
    fd = open(redirection->filename, O_RDONLY);
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
    ret = dup2(fd, 2);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    ret = evaluate_node(redirection->command);
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return ret;
}

static int eval_force_output_redir(struct ast_redirection *redirection)
{
    int fd;
    int ret = 0;
    fd = open(redirection->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }
    ret = dup2(fd, 1);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    ret = dup2(fd, 2);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    ret = evaluate_node(redirection->command);
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return ret;
}

static int eval_redir_output_input(struct ast_redirection *redirection)
{
    int fd;
    int ret = 0;
    fd = open(redirection->filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }
    ret = dup2(fd, 1);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    ret = dup2(fd, 0);
    if (ret == -1)
    {
        perror("dup2");
        return 1;
    }
    ret = evaluate_node(redirection->command);
    if (close(fd) == -1)
    {
        perror("close");
        return 1;
    }
    return ret;
}
