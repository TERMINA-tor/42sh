#include "parser.h"

// redirection = [IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' ) WORD ;

// prefix = redirection;


int is_redirection(enum token_type type)
{
    return (type == TOKEN_REDIRECT_INPUT || type == TOKEN_REDIRECT_OUTPUT
            || type == TOKEN_APPEND_OUTPUT || type == TOKEN_AMPREDIR_OUTPUT
            || type == TOKEN_AMPREDIR_INPUT
            || type == TOKEN_FORCE_OUTPUT_REDIR
            || type == TOKEN_REDIRECT_INPUT_OUTPUT);
}

enum parser_status parse_prefix(struct ast **ast, struct lexer *lexer)
{
    return parse_redirection(ast, lexer);
}

enum parser_status parse_redirection(struct ast **ast, struct lexer *lexer)
{
    struct ast_redirection *redirection = NULL;
    struct token next = lexer_peek(lexer);
    if (is_redirection(next.type))
    {
        redirection = ast_redirection_init(next.type);
        lexer_pop(lexer);
        next = lexer_peek(lexer);
    }
    else
        return PARSER_UNEXPECTED_TOKEN;

    if (next.type == TOKEN_WORD)
    {
        next = lexer_pop(lexer);
        redirection->filename = next.value;
        redirection->command = (struct ast_cmd *)*ast;
        *ast = (struct ast *)redirection;
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
