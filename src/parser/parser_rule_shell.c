#include "parser.h"

// shell_command =
//     rule_if
//     | rule_while
//     | rule_until
//     ;

enum parser_status parse_shell_command(struct ast **ast, struct lexer *lexer)
{
    struct token next = lexer_peek(lexer);
    if (next.type == TOKEN_IF)
        return parse_if(ast, lexer);
    else if (next.type == TOKEN_WHILE)
        return parse_while(ast, lexer);
    else if (next.type == TOKEN_UNTIL)
        return parse_until(ast, lexer);
    // else if (next.type == TOKEN_FOR)
    //     return parse_for(ast, lexer);
    else
        return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_if(struct ast **ast, struct lexer *lexer)
{
    if (lexer_peek(lexer).type != TOKEN_IF) // must begin with TOKEN_IF
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);
    struct ast *node = (struct ast *)ast_if_init();
    *ast = node; // do the cast here TODO
    if (parse_compound_list(&(((struct ast_if *)node)->condition), lexer)
        != PARSER_OK) // check compound list
        return PARSER_UNEXPECTED_TOKEN;
    if (lexer_peek(lexer).type != TOKEN_THEN) // check then token
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);
    if (parse_compound_list(&(((struct ast_if *)node)->then_body), lexer)
        != PARSER_OK) // check compound list
        return PARSER_UNEXPECTED_TOKEN;
    if (lexer_peek(lexer).type == TOKEN_ELSE
        || lexer_peek(lexer).type == TOKEN_ELIF) // check else token
        parse_else_clause(&(((struct ast_if *)node)->else_body), lexer);

    if (lexer_peek(lexer).type != TOKEN_FI) // check fi token
        return PARSER_UNEXPECTED_TOKEN;
    lexer_pop(lexer);
    return PARSER_OK;
}

enum parser_status parse_else_clause(struct ast **ast, struct lexer *lexer)
{
    if (lexer_peek(lexer).type == TOKEN_ELSE) // check token else
    {
        lexer_pop(lexer);

        if (parse_compound_list(ast, lexer) != PARSER_OK) // check compound list
            return PARSER_UNEXPECTED_TOKEN;
        return PARSER_OK;
    }
    else if (lexer_peek(lexer).type == TOKEN_ELIF)
    { // check elif token
        lexer_pop(lexer);
        *ast = (struct ast *)ast_if_init();

        if (parse_compound_list(&(((struct ast_if *)(*ast))->condition), lexer)
            != PARSER_OK) // check compound list
            return PARSER_UNEXPECTED_TOKEN;

        if (lexer_peek(lexer).type != TOKEN_THEN) // check then token
            return PARSER_UNEXPECTED_TOKEN;
        lexer_pop(lexer);

        if (parse_compound_list(&(((struct ast_if *)(*ast))->then_body), lexer)
            != PARSER_OK) // check compound list
            return PARSER_UNEXPECTED_TOKEN;

        if (parse_else_clause(&(((struct ast_if *)(*ast))->else_body), lexer)
            != PARSER_OK) // check else_clause
            return PARSER_UNEXPECTED_TOKEN;
        return PARSER_OK;
    }
    else
        return PARSER_UNEXPECTED_TOKEN;
}
