#include <stdio.h>
#include <stdlib.h>

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
    struct token token = lexer_pop(lexer);

    while (token.type != TOKEN_EOF)
    {
        printf("%s\n",
               tab[token.type]); // Use token.type instead of token.value
        if (token.type == TOKEN_WORD || token.type == TOKEN_REDIRECTION)
            free(token.value);
        token = lexer_pop(lexer);
    }

    if (token.type == TOKEN_EOF)
        printf("EOF\n");

    lexer_free(lexer);

    return 0;
}
