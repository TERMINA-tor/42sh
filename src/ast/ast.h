#ifndef AST_H
#define AST_H

#include <stddef.h>

enum ast_type
{
    AST_IF,
    AST_COMMAND,
    AST_SEQUENCE
};

struct ast {
    enum ast_type type;
};

struct ast_cmd {
    struct ast base;
    char **words; // NULL terminated char* list */
};

struct ast_if {
    struct ast base;
    struct ast *condition;  // the condition
    struct ast *then_body;  // the body of the then clause
    struct ast *else_body;  // the body of the else, may be NULL
};

struct ast_sequence {
    struct ast base;
    struct ast **commands;
    size_t num_commands;
};

struct ast_sequence *ast_sequence_init(void);
struct ast_if *ast_if_init(void);
struct ast_cmd *ast_cmd_init(void);
void free_ast(struct ast *node);

#endif