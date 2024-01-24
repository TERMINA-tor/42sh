#ifndef PARSER_H
#define PARSER_H

#include "../ast/ast.h"
#include "../input/input.h"
#include "../lexer/lexer.h"

enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN
};

enum parser_status parse_compound_list(struct ast **ast, struct lexer *lexer);

#endif /* ! PARSER_H */
