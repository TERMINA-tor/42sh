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

#endif /* ! PARSER_H */
