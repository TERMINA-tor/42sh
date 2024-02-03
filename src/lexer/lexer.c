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
    lexer->in_command = 0;
    return lexer;
}

// free a lexer structure
void lexer_free(struct lexer *lexer)
{
    if (lexer->fd)
        fclose(lexer->fd);
    if (lexer->least)
        free(lexer->least);
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
    char *operators = "&|<>;=\n";
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
static enum token_type get_token_type(struct lexer *lexer, char *value)
{
    const struct lookuptable table[] = { { TOKEN_EOF, "" },
                                         { TOKEN_EOL, "\n" },
                                         { TOKEN_IF, "if" },
                                         { TOKEN_ELSE, "else" },
                                         { TOKEN_THEN, "then" },
                                         { TOKEN_ELIF, "elif" },
                                         { TOKEN_FI, "fi" },
                                         { TOKEN_NOT, "!" },
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
                                         { TOKEN_FORCE_OUTPUT_REDIR, ">|" },
                                         { TOKEN_REDIRECT_INPUT_OUTPUT,
                                           "<>" } };
    size_t table_length = sizeof(table) / sizeof(struct lookuptable);
    for (size_t i = 0; i < table_length; i++)
    {
        if (!strcmp(table[i].value, value))
        {
            if (lexer->in_command && table[i].token_type >= TOKEN_IF
                && table[i].token_type <= TOKEN_DONE)
                return TOKEN_WORD;
            else
                return table[i].token_type;
        }
    }
    return TOKEN_WORD;
}

static int is_valid_variable(char c)
{
    int k = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9') || (c == '@') || (c == '*') || (c == '#');
    return k;
}

// handle the case of an unquoted ${}
static int handle_dollar(struct lexer *lexer, struct Dstring *value)
{
    char tmp = read_from_input(lexer);
    int is_embeded = 0;
    if (tmp == '{')
    {
        Dstring_append(value, '{');
        tmp = read_from_input(lexer);
        is_embeded = 1; // only $name && ${name} will be tested
    }
    while (tmp != EOF && is_valid_variable(tmp))
    {
        Dstring_append(value, tmp);
        tmp = read_from_input(lexer);
    }

    if (tmp != EOF && !is_valid_variable(tmp))
    {
        if (tmp == '}')
        {
            if (is_embeded)
            {
                is_embeded = 0;
                Dstring_append(value, '}');
            }
            else
                push_output('}', lexer);
        }
        else
            push_output(tmp, lexer);
    }
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
    int save = *is_quoted;
    *is_quoted ^= (least_quote == curr) || least_quote == -1 || (!*is_quoted && curr != least_quote);
    if (save && is_quoted)
	    return least_quote; 
    return curr;
}

// tells wether word is an assignement word or not
static int is_assignement_word(char *word)
{
    char least_quote = -1;
    int in_quote = 0;
    int is_escaped = 0;
    int is_embeded = 0; // wheter = is between {} or not
    for (size_t i = 0; word[i]; i++)
    {
        if (is_quote(word[i]))
            least_quote = handle_quotes(&in_quote, least_quote, word[i]);
        if (in_quote)
            continue;
        else if (is_escaped)
        {
            is_escaped = 0;
            continue;
        }
        else if (word[i] == '\\')
        {
            is_escaped = 1;
            continue;
        }
        else if (word[i] == '{')
        {
            is_embeded += 1;
            continue;
        }
        else if (word[i] == '}')
        {
            is_embeded -= 1;
            continue;
        }
        else if (word[i] == '=')
        {
            if (is_embeded)
                continue;
            if (i == 0)
                return 0;
            return 1;
        }
    }
    return 0;
}

static int is_valid_comment_start(char curr, char previous, char is_quoted)
{
    return curr == '#' && !is_quoted && !is_quote(previous) && (previous == -1 || is_delimitor(previous));
}

static void append_two_chars(struct Dstring *value, char curr, char tmp)
{
	Dstring_append(value, curr);
	Dstring_append(value, tmp);
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
        else if (curr == '\\' && (!is_quoted || (is_quoted && least_quote == '\"')))
        {
            char tmp = read_from_input(lexer);
            if (tmp != '\n' && (!is_quoted || (is_quoted && least_quote == '\"')))
		   append_two_chars(value, curr, tmp); 
        }
        else if ((curr == '\'' || curr == '\"'))
        {
            least_quote = handle_quotes(&is_quoted, least_quote, curr);
            Dstring_append(value, curr);
        }
        else if ((curr == '$') && (!is_quoted)) // rule_5
        {
            Dstring_append(value, curr);
            handle_dollar(lexer, value);
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
        else if (is_valid_comment_start(curr, previous, is_quoted))
            handle_comment(lexer);
        else
            Dstring_append(value, curr);

        previous = curr;
        curr = read_from_input(lexer);
    }
}

// simply returns the next available token
struct token get_next_token(struct lexer *lexer)
{
    struct Dstring *token_value = Dstring_new();
    get_next(lexer, token_value);
    Dstring_append(token_value, 0);
    struct token new_token;
    new_token.type = get_token_type(lexer, token_value->value);
    new_token.value = NULL;
    if (new_token.type != TOKEN_WORD)
    {
        Dstring_free(token_value);
    }
    else
    {
        new_token.value = token_value->value;
        free(token_value);
        if (is_assignement_word(new_token.value))
            new_token.type = TOKEN_ASSIGNEMENT;
    }

    if (new_token.type == TOKEN_SEMICOLON || new_token.type == TOKEN_EOL)
        lexer->in_command = 0;
    if (new_token.value && new_token.value[0] == '!' && !lexer->in_command)
    {
        free(new_token.value);
        new_token.value = NULL;
        new_token.type = TOKEN_NOT;
    }
    else if (new_token.type == TOKEN_WORD)
        lexer->in_command = 1;

    return new_token;
}

// returns the next available token without modifying
// the lexers offset
struct token lexer_peek(struct lexer *lexer)
{
    if (lexer->least)
        return *(lexer->least);
    struct token tok = get_next_token(lexer);
    lexer->least = calloc(1, sizeof(struct token));
    (lexer->least)->type = tok.type;
    if (tok.value)
        (lexer->least)->value = tok.value;
    return *(lexer->least);
}

// returns the next available token saving the offset
// this operation is not reversible
struct token lexer_pop(struct lexer *lexer)
{
    struct token save;
    if (lexer->least)
        save = *(lexer->least);
    else
    {
        lexer_peek(lexer);
        save = *(lexer->least);
    }
    free(lexer->least);
    lexer->least = NULL;
    return save;
}
