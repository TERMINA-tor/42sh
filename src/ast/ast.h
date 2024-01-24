#ifndef AST_H
#define AST_H

#include <stddef.h>

enum ast_type
{
    AST_IF,
    AST_COMMAND,
    AST_SEQUENCE
   // AST_UNTIL,
    //AST_FOR
};

struct ast {
    enum ast_type type;
};

struct ast_cmd {
    struct ast base;
    char **words; // NULL terminated char* list
    size_t num_words; // number word
};

struct ast_if {
    struct ast base;
    struct ast *condition;  // the condition
    struct ast *then_body;  // the body of the then clause
    struct ast *else_body;  // the body of the else, may be NULL
};

struct ast_loop {
    struct ast base;
    struct ast *condition;  // the condition
    struct ast *then_body;  // the body of the then clause
};

struct ast_sequence {
    struct ast base;
    struct ast **commands;
    size_t num_commands;
};

struct ast_sequence *ast_sequence_init(void);
struct ast_if *ast_if_init(void);
struct ast_cmd *ast_cmd_init(void);
struct ast *ast_sequence_add(struct ast *ast, struct ast *command);
struct ast *ast_if_condition_add(struct ast *ast, struct ast *condition);
struct ast *ast_if_then_add(struct ast *ast, struct ast *then);
struct ast *ast_if_else_add(struct ast *ast, struct ast *ast_else);
struct ast *ast_cmd_word_add(struct ast *ast, char *word);
void free_ast(struct ast *node);

#endif