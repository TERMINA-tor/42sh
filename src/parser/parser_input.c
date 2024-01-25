#include "parser.h"

enum parser_status parse_input(struct ast **ast, struct lexer *lexer)
{
    struct token next = lexer_peek(lexer);

    if (parse_list(ast, lexer) == PARSER_OK)
    {
        next = lexer_peek(lexer);
        if (next.type == TOKEN_EOL || next.type == TOKEN_EOF)
            return PARSER_OK;
        return PARSER_UNEXPECTED_TOKEN;
    }
    if (next.type == TOKEN_EOL || next.type == TOKEN_EOF)
        return PARSER_OK;
    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_and_or(struct ast **ast, struct lexer *lexer)
{
    return parse_pipeline(ast, lexer);
}

enum parser_status parse_list(struct ast **ast, struct lexer *lexer)
{
    if (parse_and_or(ast, lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;
    while (lexer_peek(lexer).type == TOKEN_SEMICOLON)
    {
        lexer_pop(lexer);
        if (parse_and_or(ast, lexer) != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;
    }
    if (lexer_peek(lexer).type == TOKEN_SEMICOLON)
        lexer_pop(lexer);
    return PARSER_OK;
}

enum parser_status parse_pipeline(struct ast **ast, struct lexer *lexer)
{
    return parse_command(ast, lexer);
}

enum parser_status parse_command(struct ast **ast, struct lexer *lexer)
{
    if (parse_simple_command(ast, lexer) == PARSER_OK)
        return PARSER_OK;
    return parse_shell_command(ast, lexer);
}

enum parser_status parse_simple_command(struct ast **ast, struct lexer *lexer)
{
    if (lexer_peek(lexer).type != TOKEN_WORD)
        return PARSER_UNEXPECTED_TOKEN;
    struct ast *new = ast_cmd_init();
    *ast = new;
    if (!ast_cmd_word_add(*ast, lexer_pop(lexer).value))
        return PARSER_UNEXPECTED_TOKEN;
    while (parse_element(ast, lexer) == PARSER_OK);
    return PARSER_OK;
}

enum parser_status parse_element(struct ast **ast, struct lexer *lexer)
{
    if (lexer_peek(lexer).type != TOKEN_WORD)
        return PARSER_UNEXPECTED_TOKEN;

    if (!ast_cmd_word_add(*ast, lexer_pop(lexer).value))
        return PARSER_UNEXPECTED_TOKEN;

    return PARSER_OK;
}
