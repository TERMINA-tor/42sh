#include "parser.h"

// rule_while = 'while' compound_list 'do' compound_list 'done' ;
// rule_until = 'until' compound_list 'do' compound_list 'done' ;
// rule_for = 'for' WORD ( [';'] | [ {'\n'} 'in' { WORD } ( ';' | '\n' ) ] )
// {'\n'} 'do' compound_list 'done' ;

enum parser_status parse_while(struct ast **ast, struct lexer *lexer)
{
    if (lexer_peek(lexer).type != TOKEN_WHILE)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);

    struct ast_loop *node = ast_loop_init(AST_WHILE);

    if (parse_compound_list(&(node->condition), lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;

    if (lexer_peek(lexer).type != TOKEN_DO)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);

    if (parse_compound_list(&(node->then_body), lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;

    if (lexer_peek(lexer).type != TOKEN_DONE)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);

    *ast = (struct ast *)node;
    return PARSER_OK;
}

enum parser_status parse_until(struct ast **ast, struct lexer *lexer)
{
    if (lexer_peek(lexer).type != TOKEN_UNTIL)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);

    struct ast_loop *node = ast_loop_init(AST_UNTIL);

    if (parse_compound_list(&(node->condition), lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;

    if (lexer_peek(lexer).type != TOKEN_DO)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);

    if (parse_compound_list(&(node->then_body), lexer) != PARSER_OK)
        return PARSER_UNEXPECTED_TOKEN;

    if (lexer_peek(lexer).type != TOKEN_DONE)
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);

    *ast = (struct ast *)node;
    return PARSER_OK;
}
