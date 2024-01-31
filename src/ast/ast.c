#include "ast.h"

#include <stdlib.h>

void free_ast(struct ast *node);

struct ast_sequence *ast_sequence_init(void)
{
    struct ast_sequence *new_sequence = calloc(1, sizeof(struct ast_sequence));
    if (!new_sequence)
        return NULL;
    new_sequence->base.type = AST_SEQUENCE;
    return new_sequence;
}

struct ast_pipeline *ast_pipeline_init(void)
{
    struct ast_pipeline *new_pipeline = calloc(1, sizeof(struct ast_pipeline));
    if (!new_pipeline)
        return NULL;
    new_pipeline->base.type = AST_PIPELINE;
    return new_pipeline;
}

struct ast_redirection *ast_redirection_init(enum token_type type)
{
    struct ast_redirection *new_redirection = calloc(1, sizeof(struct ast_redirection));
    if (!new_redirection)
        return NULL;
    new_redirection->base.type = AST_REDIRECTION;
    new_redirection->type = type;
    return new_redirection;
}

struct ast_if *ast_if_init(void)
{
    struct ast_if *new_if = calloc(1, sizeof(struct ast_if));
    if (!new_if)
        return NULL;
    new_if->base.type = AST_IF;
    return new_if;
}

struct ast *ast_cmd_init(void)
{
    struct ast_cmd *new_cmd = calloc(1, sizeof(struct ast_cmd));
    if (!new_cmd)
        return NULL;
    new_cmd->base.type = AST_COMMAND;
    return (struct ast *)new_cmd;
}

struct ast_loop *ast_loop_init(enum ast_type type)
{
    struct ast_loop *new_loop = calloc(1, sizeof(struct ast_loop));
    if (!new_loop)
        return NULL;
    new_loop->base.type = type;
    return new_loop;
}

struct ast *ast_sequence_add(struct ast *ast, struct ast *command)
{
    if (!command)
        return ast;
    if (!ast)
        ast = (struct ast *)ast_sequence_init();
    if (ast->type != AST_SEQUENCE)
    {
        free_ast(ast);
        return NULL;
    }
    struct ast_sequence *sequence = (struct ast_sequence *)ast;
    sequence->commands =
        realloc(sequence->commands,
                (sequence->num_commands + 1) * sizeof(struct ast *));
    if (!sequence->commands)
    {
        free_ast(ast);
        return NULL;
    }
    sequence->commands[sequence->num_commands] = command;
    sequence->num_commands++;
    return (struct ast *)sequence;
}

struct ast *ast_cmd_word_add(struct ast *ast, char *word)
{
    if (!word)
        return ast;
    if (!ast)
        ast = (struct ast *)ast_cmd_init();
    if (ast->type != AST_COMMAND)
    {
        free_ast(ast);
        return NULL;
    }
    struct ast_cmd *cmd = (struct ast_cmd *)ast;
    cmd->words = realloc(cmd->words, (cmd->num_words + 1) * sizeof(char *));
    if (!cmd->words)
    {
        free_ast(ast);
        return NULL;
    }
    cmd->words[cmd->num_words] = word;
    cmd->num_words++;
    return (struct ast *)cmd;
}

static void free_cmd(struct ast_cmd *cmd)
{
    if (cmd)
    {
        for (size_t i = 0; i < cmd->num_words; i++)
            free(cmd->words[i]);
        free(cmd->words);
        free(cmd);
    }
}

static void free_if(struct ast_if *if_node)
{
    if (if_node)
    {
        free_ast(if_node->condition);
        free_ast(if_node->then_body);
        free_ast(if_node->else_body);
        free(if_node);
    }
}

static void free_sequence(struct ast_sequence *sequence)
{
    if (sequence)
    {
        for (size_t i = 0; i < sequence->num_commands; i++)
        {
            free_ast(sequence->commands[i]);
        }
        free(sequence->commands);
        free(sequence);
    }
}

static void free_loop(struct ast_loop *loop_node)
{
    if (loop_node)
    {
        free_ast(loop_node->condition);
        free_ast(loop_node->then_body);
        free(loop_node);
    }
}

static void free_redirection(struct ast_redirection *redirection) {
    if (redirection) {
        free_cmd(redirection->command);
        free(redirection->filename);
        free(redirection);
    }
}

static void free_pipeline(struct ast_pipeline *pipeline) {
    if (pipeline) {
        free_ast(pipeline->left_cmd);
        free_ast(pipeline->right_cmd);
        free(pipeline);
    }
}

void free_ast(struct ast *node)
{
    if (node)
    {
        switch (node->type)
        {
        case AST_IF:
            free_if((struct ast_if *)node);
            break;
        case AST_COMMAND:
            free_cmd((struct ast_cmd *)node);
            break;
        case AST_SEQUENCE:
            free_sequence((struct ast_sequence *)node);
            break;
        case AST_FOR:
            free_loop((struct ast_loop *)node);
            break;
        case AST_UNTIL:
            free_loop((struct ast_loop *)node);
            break;
        case AST_WHILE:
            free_loop((struct ast_loop *)node);
            break;
        case AST_REDIRECTION:
            free_redirection((struct ast_redirection *)node);
            break;
        case AST_PIPELINE:
            free_pipeline((struct ast_pipeline *)node);
            break;

            // Ajoutez des cas pour d'autres types d'AST au besoin
        }
    }
}
