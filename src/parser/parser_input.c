#include "parser.h"


// element =
// WORD
// | redirection
// ;


enum parser_status parse_prefix(struct ast **ast, struct lexer *lexer);

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
    struct ast_sequence *sequence = ast_sequence_init();
    struct ast *node = NULL;
    if (parse_and_or(&node, lexer) != PARSER_OK)
    {
        free_ast((struct ast *)sequence);
        return PARSER_UNEXPECTED_TOKEN;
    }
    sequence =
        (struct ast_sequence *)ast_sequence_add((struct ast *)sequence, node);
    while (lexer_peek(lexer).type == TOKEN_SEMICOLON)
    {
        lexer_pop(lexer);
        struct ast *next_node;
        if (parse_and_or(&next_node, lexer) == PARSER_OK)
        {
            sequence = (struct ast_sequence *)ast_sequence_add(
                (struct ast *)sequence, next_node);
            break;
        }
    }
    if (lexer_peek(lexer).type == TOKEN_SEMICOLON)
        lexer_pop(lexer);
    *ast = (struct ast *)sequence;
    return PARSER_OK;
}

// pipeline = command { '|' {'\n'} command } ;

enum parser_status parse_pipeline(struct ast **ast, struct lexer *lexer)
{
    if (parse_command(ast, lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;
    while (lexer_peek(lexer).type == TOKEN_PIPE)
    {
        lexer_pop(lexer);
        struct ast_pipeline *pipeline = ast_pipeline_init();
        pipeline->left_cmd = *ast;
        while (lexer_peek(lexer).type == TOKEN_EOL)
        {
            lexer_pop(lexer);
        }
        if (parse_command(&pipeline->right_cmd, lexer) != PARSER_OK)
        {
            free_ast((struct ast *)pipeline);
            return PARSER_UNEXPECTED_TOKEN;
        }
        *ast = (struct ast *)pipeline;
    }
    return PARSER_OK;
}

enum parser_status parse_command(struct ast **ast, struct lexer *lexer)
{
    if (parse_simple_command(ast, lexer) == PARSER_OK)
        return PARSER_OK;
    return parse_shell_command(ast, lexer);
}

// simple_command =
// prefix { prefix }
// | { prefix } WORD { element }
// ;

enum parser_status parse_simple_command(struct ast **ast, struct lexer *lexer)
{
    struct ast *new = ast_cmd_init();
    while (parse_prefix(ast, lexer) == PARSER_OK);
    if (lexer_peek(lexer).type != TOKEN_WORD)
        goto error;
    if (!ast_cmd_word_add(new, lexer_pop(lexer).value))
        goto error;
    while (parse_element(&new, lexer) == PARSER_OK);
    *ast = new;
    return PARSER_OK;

    error:
        free_ast(new);
        return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_element(struct ast **ast, struct lexer *lexer)
{
    struct token next = lexer_peek(lexer);
    if (next.type == TOKEN_WORD)
    {
        if (!ast_cmd_word_add(*ast, lexer_pop(lexer).value))
            return PARSER_UNEXPECTED_TOKEN;
        return PARSER_OK;
    }
    return parse_redirection(ast, lexer);
}
