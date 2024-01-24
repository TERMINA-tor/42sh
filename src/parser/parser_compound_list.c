#include "parser.h"

/*
compound_list =
    {'\n'} and_or { ( ';' | '\n' ) {'\n'} and_or } [';'] {'\n'} ;
*/

void skip_newlines(struct lexer *lexer)
{
    while (lexer_peek(lexer).type == TOKEN_EOL)
        lexer_pop(lexer);
}

enum parser_status parse_compound_list(struct ast **ast, struct lexer *lexer)
{
    struct ast_sequence *sequence = ast_sequence_init();
    skip_newlines(lexer);

    struct ast *node = NULL;
    if (parse_and_or(&node, lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;

    sequence = (struct ast_sequence *)ast_sequence_add((struct ast *)sequence, node);

    while (lexer_peek(lexer).type == TOKEN_SEMICOLON
           || lexer_peek(lexer).type == TOKEN_EOL)
    {
        lexer_pop(lexer);
        skip_newlines(lexer);

        struct ast *next_node;
        if (parse_and_or(&next_node, lexer) != PARSER_OK)
            return PARSER_UNEXPECTED_TOKEN;

        sequence = (struct ast_sequence *)ast_sequence_add((struct ast *)sequence, next_node);
    }

    if (lexer_peek(lexer).type == TOKEN_SEMICOLON)
        lexer_pop(lexer);

    skip_newlines(lexer);
    *ast = (struct ast *)sequence;

    return PARSER_OK;
}
