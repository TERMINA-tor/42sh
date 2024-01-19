#include "ast.h"
#include <stdlib.h>

struct ast_sequence *ast_sequence_init(void)
{
    struct ast_sequence *new_sequence = calloc(1, sizeof(struct ast_sequence));
    if (!new_sequence)
        return NULL;
    new_sequence->base.type = AST_SEQUENCE;
    return new_sequence;
}

struct ast_if *ast_if_init(void)
{
    struct ast_if *new_if = calloc(1, sizeof(struct ast_if));
    if (!new_if)
        return NULL;
    new_if->base.type = AST_IF;
    return new_if;
}

struct ast_cmd *ast_cmd_init(void)
{
    struct ast_cmd *new_cmd = calloc(1, sizeof(struct ast_cmd));
    if (!new_cmd)
        return NULL;
    new_cmd->base.type = AST_COMMAND;
    return new_cmd;
}

void free_ast(struct ast *node);

void free_cmd(struct ast_cmd *cmd) {
    if (cmd) {
        free(cmd);
    }
}

void free_if(struct ast_if *if_node) {
    if (if_node) {
        free_ast(if_node->condition);
        free_ast(if_node->then_body);
        free_ast(if_node->else_body);
        free(if_node);
    }
}

void free_sequence(struct ast_sequence *sequence) {
    if (sequence) {
        for (size_t i = 0; i < sequence->num_commands; i++) {
            free_ast(sequence->commands[i]);
        }
        free(sequence);
    }
}

void free_ast(struct ast *node) {
    if (node) {
        switch (node->type) {
            case AST_IF:
                free_if((struct ast_if *)node);
                break;
            case AST_COMMAND:
                free_cmd((struct ast_cmd *)node);
                break;
            case AST_SEQUENCE:
                free_sequence((struct ast_sequence *)node);
                break;
            // Ajoutez des cas pour d'autres types d'AST au besoin
        }
    }
}


