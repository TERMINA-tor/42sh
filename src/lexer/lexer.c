#include "lexer.h"

struct lexer *init_lexer(FILE *fd)
{
    struct lexer *lexer = calloc(1, sizeof(struct lexer));
    if (!lexer)
    {
        fprintf(stderr, "./42sh: failed to allocate memory\n");
        return NULL;
    }
    return lexer;
}

static char read_from_input(struct lexer *lexer)
{
    char curr = fgetc(lexer->fd);
    lexer->offset++;
}

static int is_delimitor(char c)
{
    char *delimitors = "&|\'\"<>";
    for (int i = 0; i < 6; i++)
    {
        if (delimitors[i] == c)
            return 1;
    }
    return 0;
}

static char *append_to_string(char *string, char c, size_t len)
{
    string = realloc(string, (len + 1) * sizeof(char));
    string[len] = c;
    return string;
}

static get_token_type(char *value)
{
    const lookuptable table[] = { { TOKEN_IF, "if" },
                                  { TOKEN_ELSE, "else" },
                                  { TOKEN_THEN, "then" },
                                  { TOKEN_ELIF, "elif" },
                                  { TOKEN_FI, "fi" },
                                  { TOKEN_WHILE, "while" },
                                  { TOKEN_UNTILL, "untill" },
                                  { TOKEN_FOR, "for" },
                                  { TOKEN_DO, "do" },
                                  { TOKEN_DONE, "done" },
                                  { TOKEN_AND, "&&" },
                                  { TOKEN_OR, "||" },
                                  { TOKEN_REDIRECT_INPUT, "<" },
                                  { TOKEN_REDIRECT_OUTPUT, "<" },
                                  { TOKEN_APPEND_OUTPUT, ">>" },
                                  { TOKEN_AMPREDIR_OUTPUT, ">&" },
                                  { TOKEN_AMPREDIR_INPUT, "<&" },
                                  { TOKEN_FORCE_OUTPUT_REDIR, ">|" } };
    size_t table_length = sizeof(table) / sizeof(lookuptable);
    for (size_t i = 0; i < table_length; i++)
    {
        if (!strcmp(table[i].value, value))
            return table[i].token_type;
    }
    return TOKEN_WORD;
}

struct token get_next_token(struct lexer *lexer)
{
    struct token tok; // the token to be returned
    char *token_value = NULL; // its value
    char curr = read_from_input(lexer); // reads the first character

    while (curr == ' ') // skip all white spaces
        curr = read_from_input(lexer);

    while (is_delimitor(c)) // try to read the next special token
    {
        if (!append_to_string(token_value, c))
            goto error;
        c = read_from_intput(lexer);
    }
    if (!token_value) // in case no delimitor was found
    {
        while (!is_delimitor(c)) // read the next word
        {
            if (!append_tok_string(token_value, c))
                goto error;
            c = read_from_input(lexer);
        }
    }
    if (!append_to_string(token_value, 0))
        goto error;

    tok.type = get_token_type(value); // get the token type using a LUT
    if (tok.type == TOKEN_COMMAND || tok.type == TOKEN_WORD)
    {
        tok.value = token_value;
    }
    else
        free(value);
    return tok;

error:
    fprintf(stderr, "./42sh: failed to allocate memory\n");
    tok.type = TOKEN_ERROR;
    return token;
}
