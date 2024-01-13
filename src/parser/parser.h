#ifndef PARSER_H
#define PARSER_H

#include "../ast/ast.h"
#include "../lexer/lexer.h"

enum parser_status
{
        PARSER_OK,
        PARSER_UNEXPECTED_TOKEN,
};

/**
 ** \brief parse input rule
 * input =
 *      list '\n'
 *    | list EOF
 *    | '\n'
 *    | EOF
 */
enum parser_status parse_input(struct ast **res, struct lexer *lexer);

#endif /* ! PARSER_H */