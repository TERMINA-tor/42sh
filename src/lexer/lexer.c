#include "lexer.h"

// initialize a lexer structure
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

// free a lexer structure
void lexer_free(struct lexer *lexer)
{
    if (lexer->fd)
        fclose(lexer->fd);
    free(lexer);
}

// reads one character from the stream pointed at by lexer->fd
// and updated the lexer offset
static char read_from_input(struct lexer *lexer)
{
    if (!lexer->fd)
        return EOF;
    char curr = fgetc(lexer->fd);
    if (curr != EOF)
    	lexer->offset++;
    return curr;
}

// pushes c in the buffer
// i.e: the next getc will return c
static void push_output(char c, struct lexer *lexer)
{
    ungetc(c, lexer->fd);
    lexer->offset--;
}

// simply tells wether c is a character
// that can be used in an operator or not
static int is_operator(char c)
{
    char *operators = "&|<>;\n";
    for (size_t i = 0; operators[i]; i++)
    {
        if (c == operators[i])
            return 1;
    }
    return 0;
}

// simply tells wether c is a character
// that can be used in a delimitor or not
static int is_delimitor(char c)
{
    char *delimitors = " #\r\t\\";
    for (size_t i = 0; delimitors[i]; i++)
    {
        if (c == delimitors[i])
            return 1;
    }
    return 0;
}

// simply tells wether c is a character
// that can be used in blank or not
static int is_blank(char c)
{
    return (c == '\r') || (c == '\t') || (c == ' ');
}

// simply tells wether c is a character
// that can be used in a quote or not
static int is_quote(char c)
{
    return (c == '\'') || (c == '\"');
}

// return the type of the token valu
// using a lookup table
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
                                         { TOKEN_PIPE, "|" },
                                         { TOKEN_SEMICOLON, ";" },
                                         { TOKEN_REDIRECT_INPUT, "<" },
                                         { TOKEN_REDIRECT_OUTPUT, ">" },
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

// handle the case of an unquoted ${}
static int handle_dollar(struct lexer *lexer, struct Dstring *value)
{
    char tmp = read_from_input(lexer);
    if (tmp != '{')
    {
        push_output(tmp, lexer);
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

// discards a comment
// i.e: discarding every character betwenn # and <newline> excluded
static void handle_comment(struct lexer *lexer)
{
    char tmp = read_from_input(lexer);
    while (tmp != '\n' && tmp != EOF)
        tmp = read_from_input(lexer);
    if (tmp == '\n')
        push_output('\n', lexer);
}

// handles the is_quoted flag (set to 1 if curr is the first encountered quote
// or set to 0 if the matching quote is found)
static char handle_quotes(int *is_quoted, char least_quote, char curr)
{
    *is_quoted ^= (least_quote == curr) || least_quote == -1;
    return *is_quoted ? curr : least_quote;
}

static void get_next(struct lexer *lexer, struct Dstring *value)
{
    char previous = -1; // previous character
    char curr = read_from_input(lexer);
    char least_quote = -1; // keeps track of the last quote type (\' | \")
    int is_quoted = 0;
    while (curr != EOF) // rule_1 
    {
	// add condition for eof
        if (is_operator(previous) & !is_quoted)
        {
            if (is_operator(curr) && curr != '\n' && previous != '\n') // rule_2
                Dstring_append(value, curr);
            else // rule_3
            {
                push_output(curr, lexer);
                break;
            }
        }
        else if (curr == '\\' && !is_quoted)
        {
            char tmp = read_from_input(lexer);
            if (tmp != '\n' && (!is_quoted))
                push_output(tmp, lexer); // \\n = line continuation
        }
        else if ((curr == '\'' || curr == '\"') && !is_delimitor(previous))
        {
            least_quote = handle_quotes(&is_quoted, least_quote, curr);
            Dstring_append(value, curr);
        }
        else if ((curr == '$') && (!is_quoted)) // rule_5
        {
            Dstring_append(value, curr);
            if (handle_dollar(lexer, value))
                break;
        }
        else if ((!is_quoted) && is_operator(curr) && !is_delimitor(previous))
        {
            if (previous == -1)
                Dstring_append(value, curr);
            else if (!is_operator(previous))
            {
                push_output(curr, lexer);
                break;
            }
        }
        else if ((!is_quoted) && (is_blank(curr)))
        {
            if (previous != -1 && !is_blank(previous))
                break;
        }
        else if ((!is_operator(curr)) && (!is_delimitor(curr)))
            Dstring_append(value, curr);
        else if (curr == '#' && !is_quoted && !is_quote(previous))
            handle_comment(lexer);
        else
            Dstring_append(value, curr);

        previous = curr;
        curr = read_from_input(lexer);
    }
    Dstring_append(value, 0);
}

// simply returns the next available token
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

// returns the next available token without modifying
// the lexers offset
struct token lexer_peek(struct lexer *lexer)
{
    size_t old_offset = lexer->offset;

    struct token tok = get_next_token(lexer);
    if (tok.type == TOKEN_WORD)
        free(tok.value);

    size_t offset = lexer->offset - old_offset;
    fseek(lexer->fd, (long)(-offset), SEEK_CUR);
    lexer->offset = old_offset;
    return tok;
}

// returns the next available token saving the offset
// this operation is not reversible
struct token lexer_pop(struct lexer *lexer)
{
    return get_next_token(lexer);
}
