#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lexer.h"

char *tab[] = {
    [TOKEN_IF] = "if",       [TOKEN_ELSE] = "else",  [TOKEN_ELIF] = "elif",
    [TOKEN_THEN] = "then",   [TOKEN_QUOTE] = "'",    [TOKEN_WORD] = "word",
    [TOKEN_SEMICOLON] = ";", [TOKEN_FI] = "fi",      [TOKEN_EOF] = "EOF",
    [TOKEN_EOL] = "\n",      [TOKEN_DO] = "do", [TOKEN_DONE] = "done",
    [TOKEN_FOR] = "for",    [TOKEN_IN] = "in",      [TOKEN_WHILE] = "while",
    [TOKEN_PIPE] = "|",     [TOKEN_UNTIL] = "until", [TOKEN_REDIRECTION] = "redirection"
};

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    struct lexer *lexer = lexer_new(argv[1]);
    struct token token = lexer_peek(lexer);

    size_t len = strlen(argv[1]);

    for (size_t i = 0; i < len; i++)
    {
        printf("%s\n",
               tab[token.type]); // Use token.type instead of token.value
        if (token.type == TOKEN_WORD)
            free(token.value);
        token = lexer_peek(lexer);
        if (i + 1 == len)
            if (token.type == TOKEN_WORD)
                free(token.value);
    }

    if (token.type == TOKEN_EOF)
        printf("EOF\n");

    lexer_free(lexer);

    return 0;
}