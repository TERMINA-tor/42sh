#include <stdio.h>

#include "../src/lexer/lexer.h"

char *tab[] = {
    [TOKEN_IF] = "if",
    [TOKEN_ELSE] = "else",
    [TOKEN_ELIF] = "elif",
    [TOKEN_THEN] = "then",
    [TOKEN_QUOTE] = "'",
    [TOKEN_WORD] = "word",
    [TOKEN_SEMICOLON] = ";",
    [TOKEN_FI] = "fi",
    [TOKEN_EOF] = "EOF",
    [TOKEN_EOL] = "\n",
    [TOKEN_ERROR] = "error"
};

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    struct lexer *lexer = lexer_new(argv[1]);
    struct token token = lexer_pop(lexer);

    while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR)
    {
        printf("%s\n", tab[token.type]); // Use token.type instead of token.value

        token = lexer_pop(lexer);
    }

    if (token.type == TOKEN_EOF)
        printf("EOF\n");

    lexer_free(lexer);

    return 0;
}
