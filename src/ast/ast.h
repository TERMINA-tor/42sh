#ifndef AST_H
#define AST_H

#include <stddef.h>

enum ast_type
{
    AST_IF,
    AST_COMMAND,
    AST_SEQUENCE
    AST_UNTIL,
    AST_FOR
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
    struct ast **commands; // list of commands
    size_t num_commands;    // number of commands
};

struct ast_sequence *ast_sequence_init(void); // init the sequence, list of command
struct ast_if *ast_if_init(void);   // init if node
struct ast_cmd *ast_cmd_init(void);   // init commands node
struct ast_loop *ast_loop_init(enum ast_type type); // init loop node
struct ast_loop *ast_loop_condition_add(enum ast_type type, struct ast *ast, struct ast *condition);    // add condition to loop node
struct ast *ast_loop_body_add(enum ast_type type, struct ast *ast, struct ast *body);   // add body to loop
struct ast *ast_sequence_add(struct ast *ast, struct ast *command); // add another command to the list of command
struct ast *ast_if_condition_add(struct ast *ast, struct ast *condition);   // add condition to if node
struct ast *ast_if_then_add(struct ast *ast, struct ast *then); // add then_body to if node
struct ast *ast_if_else_add(struct ast *ast, struct ast *ast_else); // add else_body to if node
struct ast *ast_cmd_word_add(struct ast *ast, char *word); // add words to the list of words in command node
void free_ast(struct ast *node);    // free all the ast

#endif