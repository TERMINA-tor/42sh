#ifndef RULE_SHELL_H
#define RULE_SHELL_H

#include "../ast/ast.h"
#include "../input/input.h"
#include "../lexer/lexer.h"

enum parser_status
{
	PARSER_OK,
	PARSER_UNEXPECTED_TOKEN
};

enum parser_status parse_shell_command(struct ast **ast, struct lexer *lexer);

enum parser_status parse_if(struct ast **ast, struct lexer *lexer);

enum parser_status parse_else_clause(struct ast **ast, struct lexer *lexer);
#endif /* ! RULE_SHELL_H */
