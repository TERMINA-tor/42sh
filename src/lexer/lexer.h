#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_ELIF,
    TOKEN_THEN,
    TOKEN_FI,
    TOKEN_QUOTE,
    TOKEN_WORD,
    TOKEN_SEMICOLON,
    TOKEN_EOF,
    TOKEN_EOL,
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

#endif /* ! TOKEN_H */
