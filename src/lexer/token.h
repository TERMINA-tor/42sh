#ifndef TOKEN_H
#define TOKEN_H

#include <unistd.h>

enum token_type
{
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_ELIF,
    TOKEN_THEN,
    TOKEN_FI,
    TOKEN_QUOTE,
    TOKEN_DOUBLE_QUOTE,
    TOKEN_WORD,
    TOKEN_SEMICOLON,
    TOKEN_EOF,
    TOKEN_EOL,
    TOKEN_REDIRECTION,
    TOKEN_PIPE,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_IN,
    TOKEN_DO,
    TOKEN_DONE,
    TOKEN_UNTIL,
    TOKEN_AND,
    TOKEN_NOT
};

struct token
{
    enum token_type type; // The kind of token
    char *value; // its value
};
#endif /* !TOKEN_H */
