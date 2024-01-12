#include "parser.h"

enum parser_status parse_input(struct ast **res, struct lexer *lexer)
{
	*res = calloc(1, sizeof(struct ast));
	struct token next = lexer_peek(lexer);
	
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
	    if (parse_list(res, lexer) != PARSER_OK)
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

enum parser_status parse_list(struct ast **res, struct lexer *lexer)
{
	if (parse_and_or(res, lexer) == PARSER_OK)
	{
		if (lexer_peek(lexer).type == TOKEN_SEMICOLON)
		{
			while (lexer_peek(lexer).type == TOKEN_SEMICOLON)
			{
				lexer_pop(lexer);
				if (parse_and_or(res, lexer) != PARSER_OK)
					break;
			}
		}	
		return PARSER_OK;
	}
	return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_and_or(struct ast **res, struct lexer *lexer)
{
	return parse_pipeline(res, lexer);
}

enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer)
{
	return parse_command(res, lexer);
}

enum parser_status parse_command(struct ast **res, struct lexer *lexer)
{
	// no if/else handling yet
	*res = ast_new(AST_COMMAND);
	if (!*res)
		return PARSER_UNEXPECTED_TOKEN;
	(*res)->value = lexer_pop(lexer).value;
	return parse_simple_command(res, lexer);
}

enum parser_status parse_simple_command(struct ast **res, struct lexer *lexer)
{
	struct ast *word;
	struct ast *command = *res;
	size_t new_size = command->nbchildren * sizeof(struct ast);
	if (lexer_peek(lexer).type == TOKEN_WORD)
	{
		command->nbchildren += 1;
		new_size = command->nbchildren * sizeof(struct ast);
		command->children = realloc(command->children, new_size);
		word = ast_new(AST_WORD);
		word->value = lexer_pop(lexer).value;
		command->children[command->nbchildren - 1] = word;
		while (parse_element(res, lexer) == PARSER_OK);
		return PARSER_OK;
	}
	return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_element(struct ast **res, struct lexer *lexer)
{
	struct ast *word;
	struct ast *command = *res;
	size_t new_size = command->nbchildren * sizeof(struct ast);
	if (lexer_peek(lexer).type == TOKEN_WORD)
	{
		command->nbchildren += 1;
		new_size = command->nbchildren * sizeof(struct ast);
		command->children = realloc(command->children, new_size);
		if (!command->children)
			return PARSER_UNEXPECTED_TOKEN;

		word = ast_new(AST_WORD);
		word->value = lexer_pop(lexer).value;	
		command->children[command->nbchildren - 1] = word;
		return PARSER_OK;
	}
	return PARSER_UNEXPECTED_TOKEN;
}
