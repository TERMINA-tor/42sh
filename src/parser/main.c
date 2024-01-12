#include <stdio.h>

#include "parser.h"

#include "../input/input.h"

int main(int argc, char **argv)
{
	char *input = get_input(argc, argv);
	struct lexer *lexer = lexer_new(input);
	struct ast *res = NULL;
	enum parser_status result = parse_input(&res, lexer);
	if (!res)
		return 1;
	return result;
}
