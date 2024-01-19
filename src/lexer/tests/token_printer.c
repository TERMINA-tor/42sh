#include <stdio.h>
#include <stdlib.h>

#include "../lexer.h"
#include "../../input/input.h"

char *tab[] = {
    [TOKEN_IF] = "if",       [TOKEN_ELSE] = "else",  [TOKEN_ELIF] = "elif",
    [TOKEN_THEN] = "then",   [TOKEN_WORD] = "word", [TOKEN_SEMICOLON] = ";", 
    [TOKEN_FI] = "fi",      [TOKEN_EOF] = "EOF", [TOKEN_EOL] = "\n"
};

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    FILE *fd = get_input(argc, argv);

    struct lexer *lexer = init_lexer(fd);
    struct token token = lexer_pop(lexer);

    while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR)
    {
        printf("%s\n",
               tab[token.type]); // Use token.type instead of token.value
        if (token.type == TOKEN_WORD)
            free(token.value);
        token = lexer_pop(lexer);
    }

    if (token.type == TOKEN_EOF)
        printf("EOF\n");

    lexer_free(lexer);

    return 0;
}