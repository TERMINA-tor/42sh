#ifndef AST_H
#define AST_H

#include <stddef.h>

#include "../lexer/lexer.h"

enum ast_type
{
    AST_IF,
    AST_COMMAND,
    AST_SEQUENCE,
    AST_UNTIL,
    AST_WHILE,
    AST_FOR,
    AST_REDIRECTION,
    AST_PIPELINE,
    AST_NOT
};

struct ast
{
    enum ast_type type;
};

struct ast_cmd
{
    struct ast base;
    char **words; // NULL terminated char* list
    size_t num_words; // number word
};

struct ast_if
{
    struct ast base;
    struct ast *condition; // the condition
    struct ast *then_body; // the body of the then clause
    struct ast *else_body; // the body of the else, may be NULL
};

struct ast_loop
{
    struct ast base;
    struct ast *condition; // the condition
    struct ast *then_body; // the body of the then clause
};

struct ast_sequence
{
    struct ast base;
    struct ast **commands; // list of commands
    size_t num_commands; // number of commands
};

struct ast_redirection
{
    struct ast base;
    struct ast_cmd *command;    // the command to redirect
    char **filenames; // the filenames to redirect to
    int num_filenames; // the number of filenames
    enum token_type type; // the type of redirection
};

struct ast_pipeline {
    struct ast base;
    struct ast *left_cmd;   // the left command
    struct ast *right_cmd;  // the right command
};

struct ast_not {
    struct ast base;
    struct ast *command;    // the command to negate
};

struct ast_sequence *ast_sequence_init(void);   // init the sequence, list of command
struct ast_redirection *ast_redirection_init(enum token_type type);   // init redirection node
struct ast_pipeline *ast_pipeline_init(void);   // init pipeline node
struct ast_not *ast_not_init(void);   // init not node
struct ast_if *ast_if_init(void);   // init if node
struct ast *ast_cmd_init(void);   // init commands node
struct ast_loop *ast_loop_init(enum ast_type type); // init loop node
struct ast *ast_sequence_add(
    struct ast *ast,
    struct ast *command); // add another command to the list of command, if ast
                          // not found init a sequence node and add the sequence
struct ast *ast_cmd_word_add(
    struct ast *ast,
    char *word); // add words to the list of words in command node, if ast not
                 // found init a command node and add the word to the list
void free_ast(struct ast *node); // free all the ast

#endif
