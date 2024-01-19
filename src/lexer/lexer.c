#include "lexer.h"

struct lexer *init_lexer(FILE *fd)
{
    struct lexer *lexer = calloc(1, sizeof(struct lexer));
    if (!lexer)
    {
        fprintf(stderr, "./42sh: failed to allocate memory\n");
        return NULL;
    }
    lexer->fd = fd;
    return lexer;
}

void lexer_free(struct lexer *lexer)
{
    fclose(lexer->fd);
    free(lexer);
}

static struct Dstring *Dstring_new(void)
{
    return calloc(1, sizeof(struct Dstring));
}

static void Dstring_append(struct Dstring *str, char c)
{
    str->value = realloc(str->value, str->size + 1);
    str->value[str->size] = c;
    str->size++;
}

static void Dstring_free(struct Dstring *str)
{
    free(str->value);
    free(str);
}

static char read_from_input(struct lexer *lexer)
{
    char curr = fgetc(lexer->fd);
    lexer->offset++;
    return curr;
}

static void push_output(char c, struct lexer *lexer)
{
    ungetc(c, lexer->fd);
    lexer->offset--;
}

static int is_operator(char c)
{
    char *operators = "&|<>;";
    for (size_t i = 0; operators[i]; i++)
    {
        if (c == operators[i])
            return 1;
    }
    return 0;
}

static int is_blank(char c)
{
    return (c == '\r') || (c == '\t') || (c == ' ');
}

static enum token_type get_token_type(char *value)
{
    const struct lookuptable table[] = { { TOKEN_EOF, "" },
                                         { TOKEN_EOL, "\n" },
                                         { TOKEN_IF, "if" },
                                         { TOKEN_ELSE, "else" },
                                         { TOKEN_THEN, "then" },
                                         { TOKEN_ELIF, "elif" },
                                         { TOKEN_FI, "fi" },
                                         { TOKEN_WHILE, "while" },
                                         { TOKEN_UNTIL, "until" },
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
    size_t table_length = sizeof(table) / sizeof(struct lookuptable);
    for (size_t i = 0; i < table_length; i++)
    {
        if (!strcmp(table[i].value, value))
            return table[i].token_type;
    }
    return TOKEN_WORD;
}

static int handle_dollar(struct lexer *lexer, struct Dstring *value)
{
    char tmp = read_from_input(lexer);
    if (tmp != '{')
    {
        push_output('{', lexer);
        return 0;
    }
    while (tmp != EOF && tmp != '}')
    {
        Dstring_append(value, tmp);
    }
    if (tmp == '}')
        push_output('}', lexer);
    return 1;
}

static void handle_comment(struct lexer *lexer)
{
    char tmp = read_from_input(lexer);
    while (tmp != '\n' && tmp != EOF)
        tmp = read_from_input(lexer);
    if (tmp == '\n')
        push_output('\n', lexer);
}

static void get_next(struct lexer *lexer, struct Dstring *value)
{
    char previous = -1;
    char curr = read_from_input(lexer);
    int is_quoted = 0;
    int is_comment = 0;
    while (curr != EOF)
    { 
        if (is_comment) // add this block
        {
            if (curr == '\n' || curr == EOF)
                is_comment = 0;
            curr = read_from_input(lexer);
            continue;
        }
        if (curr == '#') // modify this block
        {
            is_comment = 1;
            curr = read_from_input(lexer);
            continue;
        }
        if (is_operator(previous))
        {
            if (is_operator(curr) && (!is_quoted))
                Dstring_append(value, curr);
            else
            {
                push_output(curr, lexer);
                break;
            }
        }
        else if (curr == '\\')
        {
            char tmp = read_from_input(lexer);
            if (tmp != '\n' && (!is_quoted))
                push_output(tmp, lexer);
        }
        else if (curr == '\'' || curr == '\"')
        {
            is_quoted ^= 1;
            Dstring_append(value, curr);
        }
        else if ((curr == '$') && (!is_quoted))
        {
            Dstring_append(value, curr);
            if (handle_dollar(lexer, value))
                break;
        }
        else if ((!is_quoted) && is_operator(curr))
        {
            if (previous != -1)
            {
                push_output(curr, lexer);
                break;
            }
            Dstring_append(value, curr);
        }
        else if ((!is_quoted) && is_blank(curr))
        {
            if (previous != -1)
                break;
        }
        else if ((!is_operator(curr)))
            Dstring_append(value, curr);
        else if (curr == '#')
            handle_comment(lexer);
        else
            Dstring_append(value, curr);
    
        previous = curr;
        curr = read_from_input(lexer);
    }
    Dstring_append(value, 0);
}

struct token get_next_token(struct lexer *lexer)
{
    struct Dstring *token_value = Dstring_new();
    get_next(lexer, token_value);

    struct token new_token;
    new_token.type = get_token_type(token_value->value);
    if (new_token.type != TOKEN_WORD)
    {
        Dstring_free(token_value);
    }
    else
    {
	new_token.value = token_value->value;
        free(token_value);
    }
    return new_token;
}

struct token lexer_peek(struct lexer *lexer)
{
    size_t old_offset = lexer->offset;

    struct token tok = get_next_token(lexer);
    if (tok.type == TOKEN_WORD)
        free(tok.value);

    size_t offset = lexer->offset - old_offset;
    fseek(lexer->fd, -offset, SEEK_CUR);
    return tok;
}

struct token lexer_pop(struct lexer *lexer)
{
    return get_next_token(lexer);
}
