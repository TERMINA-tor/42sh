#include "parser.h"

enum parser_status parse_input(struct ast **res, struct lexer *lexer)
{
	*res = calloc(1, sizeof(struct ast));
	struct token next = lexer_peek(lexer).type;
	
	switch (next.type)
	{
	case TOKEN_EOL:
	    lexer_pop(lexer);
	    return PARSER_OK;
	    break;
	case TOKEN_EOF:
	    lexer_pop(lexer);
	    return PARSER_OK;
	    break;
	default:
	    if (parse_list(ast, lexer) != PARSER_UNEXPECTED_TOKEN)
	    {
		return PARSER_UNEXPECTED_TOKEN;
	    }
	    struct token last = lexer_pop(lexer);
	    if (last.type == TOKEN_EOF || last.type == TOKEN_EOL)
		return PARSER_OK;
	    return PARSER_UNEXPECTED_TOKEN;
	    break;
	}
	
}
