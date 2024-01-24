#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum token_type
{
    TOKEN_INIT,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_ELIF,
    TOKEN_THEN,
    TOKEN_FI,
    TOKEN_NOT,
    TOKEN_PIPE,
    TOKEN_WHILE,
    TOKEN_UNTIL,
    TOKEN_FOR,
    TOKEN_DO,
    TOKEN_DONE,
    TOKEN_WORD,
    TOKEN_SEMICOLON,
    TOKEN_EOF,
    TOKEN_EOL,
    TOKEN_OR,
    TOKEN_AND,
    TOKEN_REDIRECT_INPUT,
    TOKEN_REDIRECT_OUTPUT,
    TOKEN_APPEND_OUTPUT,
    TOKEN_AMPREDIR_OUTPUT,
    TOKEN_AMPREDIR_INPUT,
    TOKEN_FORCE_OUTPUT_REDIR,
    TOKEN_ERROR
};

// structure representing a keyword in an input
struct token
{
    enum token_type type; // the type of the token
    char *value; // it's value, in case it's a word or a command
};

struct lexer
{
    FILE *fd; // the file descriptor pointing to the input stream
    size_t offset; // how many characters have been read
};

// lookuptable structure
struct lookuptable
{
    enum token_type token_type; // type of the token
    char *value; // the value associated to it
};

// structure representing a string
struct Dstring
{
    char *value;
    size_t size;
};

/*
 * returns a dynamically allocated lexer structure
 */
struct lexer *init_lexer(FILE *fd);

/*
 * free a lexer structure
 */

void lexer_free(struct lexer *lexer);

/*
 * returns the next token
 * this function does not modify the offset
 */
struct token lexer_peek(struct lexer *lexer);

/*
 * returns the next token
 * this function modifies the offset
 */
struct token lexer_pop(struct lexer *lexer);

#endif /* ! LEXER_H */
