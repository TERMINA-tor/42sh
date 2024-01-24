#ifndef PARSER_H
#define PARSER_H

#include "../lexer/lexer.h"
#include "../ast/ast.h"
#include "../input/input.h"

enum parser_status
{
	PARSER_OK,
	PARSER_UNEXPECTED_TOKEN
};

enum parser_status parse_input(struct ast **ast, struct lexer *lexer);

enum parser_status parse_and_or(struct ast **ast, struct lexer *lexer);

enum parser_status parse_list(struct ast **ast, struct lexer *lexer);

enum parser_status parse_pipeline(struct ast **ast, struct lexer *lexer);

enum parser_status parse_command(struct ast **ast, struct lexer *lexer);

enum parser_status parse_simple_command(struct ast **ast, struct lexer *lexer);

enum parser_status parse_element(struct ast **ast, struct lexer *lexer);

#endif /* ! PARSER_H */
