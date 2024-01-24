#include "../input/input.h"
#include "lexer.h"

void lut(struct token t)
{
    const struct lookuptable table[] = {
        { TOKEN_EOF, "TOKEN_EOF" },
        { TOKEN_EOL, "TOKEN_EOL" },
        { TOKEN_IF, "TOKEN_IF" },
        { TOKEN_ELSE, "TOKEN_ELSE" },
        { TOKEN_THEN, "TOKEN_THEN" },
        { TOKEN_ELIF, "TOKEN_ELIF" },
        { TOKEN_FI, "TOKEN_FI" },
        { TOKEN_WHILE, "TOKEN_WHILE" },
        { TOKEN_UNTIL, "TOKEN_UNTILL" },
        { TOKEN_FOR, "TOKEN_FOR" },
        { TOKEN_DO, "TOKEN_DO" },
        { TOKEN_SEMICOLON, "TOKEN_SEMICOLON" },
        { TOKEN_DONE, "TOKEN_DONE" },
        { TOKEN_AND, "TOKEN_AND" },
        { TOKEN_OR, "TOKEN_OR" },
        { TOKEN_PIPE, "TOKEN_PIPE" },
        { TOKEN_REDIRECT_INPUT, "TOKEN_REDIR_LEFT" },
        { TOKEN_REDIRECT_OUTPUT, "TOKEN_REDIR_RIGHT" },
        { TOKEN_APPEND_OUTPUT, "TOKEN_APPEND" },
        { TOKEN_AMPREDIR_OUTPUT, "TOKEN_AMPREDIR_OUTPUT" },
        { TOKEN_AMPREDIR_INPUT, "TOKEN_AMPREDIR_INPUT" },
        { TOKEN_FORCE_OUTPUT_REDIR, "TOKEN_FORCE_REDIR" }
    };
    size_t table_length = sizeof(table) / sizeof(struct lookuptable);
    for (size_t i = 0; i < table_length; i++)
    {
        if (table[i].token_type == t.type)
        {
            printf("%s\n", table[i].value);
            return;
        }
    }
    printf("%s\n", t.value);
    free(t.value);
}

int main(int argc, char **argv)
{
    FILE *fd = get_input(argc, argv);
    struct lexer *lexer = init_lexer(fd);
    struct token t;
    do
    {
        t = lexer_pop(lexer);
        lut(t);
    } while (t.type != TOKEN_EOF);
    lexer_free(lexer);
}
