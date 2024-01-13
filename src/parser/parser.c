#include "parser.h"

static enum parser_status parse_list(struct ast **res, struct lexer *lexer);

static enum parser_status parse_and_or(struct ast **res, struct lexer *lexer);

static enum parser_status parse_command(struct ast **res, struct lexer *lexer);

static enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer);

static enum parser_status parse_simple_command(struct ast **res, struct lexer *lexer);

static enum parser_status parse_element(struct ast **res, struct lexer *lexer);

static enum parser_status parse_shell_command(struct ast **res, struct lexer *lexer);

static enum parser_status parse_rule_if(struct ast **res, struct lexer *lexer);

static enum parser_status parse_compound_list(struct ast **res, struct lexer *lexer);

static enum parser_status parse_and_or(struct ast **res, struct lexer *lexer);

static enum parser_status parse_else(struct ast **res, struct lexer *lexer);

enum parser_status parse_input(struct ast **res, struct lexer *lexer)
{
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

static enum parser_status parse_list(struct ast **res, struct lexer *lexer)
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

static enum parser_status parse_and_or(struct ast **res, struct lexer *lexer)
{
	return parse_pipeline(res, lexer);
}

static enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer)
{
	return parse_command(res, lexer);
}

static enum parser_status parse_command(struct ast **res, struct lexer *lexer)
{
	if (parse_shell_command(res, lexer) != PARSER_OK)
		return parse_simple_command(res, lexer);
	return PARSER_OK;
}

static enum parser_status parse_simple_command(struct ast **res, struct lexer *lexer)
{
	if (lexer_peek(lexer).type == TOKEN_WORD)
	{
		*res = ast_new(AST_COMMAND);
		struct ast *command = *res;
		command->value = lexer_pop(lexer).value;
		if (lexer_peek(lexer).type == TOKEN_WORD)
		{
			while (parse_element(res, lexer) == PARSER_OK);
		}
		return PARSER_OK;
	}
	return PARSER_UNEXPECTED_TOKEN;
}

static enum parser_status parse_element(struct ast **res, struct lexer *lexer)
{
	struct ast *command = *res;
	size_t new_size = command->nbchildren * sizeof(struct ast);
	if (lexer_peek(lexer).type == TOKEN_WORD)
	{
		command->nbchildren += 1;
		new_size = command->nbchildren * sizeof(struct ast);
		command->children = realloc(command->children, new_size);
		if (!command->children)
			return PARSER_UNEXPECTED_TOKEN;

		struct ast *word = ast_new(AST_WORD);
		if (!word)
			return PARSER_UNEXPECTED_TOKEN;
		word->value = lexer_pop(lexer).value;	
		command->children[command->nbchildren - 1] = word;
		return PARSER_OK;
	}
	return PARSER_UNEXPECTED_TOKEN;
}

/* static enum parser_status parse_shell_command(struct ast **res, struct lexer *lexer)
{
	if (*res && (*res)->type == AST_ELIF)
		return parse_rule_if (res, lexer);
	struct ast *tmp = ast_new(AST_ELIF); // to test the next expression
	if (parse_rule_if(&tmp, lexer) != PARSER_OK) // if it is not an if rule
	{
		ast_free(tmp); // we do not need the temporary child anymore
		return PARSER_UNEXPECTED_TOKEN;
	}
	// add_child_ast(*res, tmp); // add tmp as child of res
	*res = tmp;
	return PARSER_OK;
}

static enum parser_status parse_rule_if(struct ast **res, struct lexer *lexer)
{
	
	if (lexer_peek(lexer).type != TOKEN_IF) // must begin with TOKEN_IF
		return PARSER_UNEXPECTED_TOKEN;
	lexer_pop(lexer);
	
	struct ast *command = ast_new(AST_IF);
	struct ast *condition = NULL;
	if (parse_compound_list(&condition, lexer) != PARSER_OK) // followed by complist
		goto error;

	if (add_child_ast(command, condition))
		goto error;

	if (lexer_peek(lexer).type != TOKEN_THEN) // followed by TOKEN_THEN
		return PARSER_UNEXPECTED_TOKEN;

	lexer_pop(lexer);

	if (parse_compound_list(&condition, lexer) != PARSER_OK) // compound list
		goto error;

	if (add_child_ast(command, condition))
		goto error;

	parse_else(res, lexer);
	
	if (command)
		add_child_ast(*res, command);
	
	if (lexer_peek(lexer).type != TOKEN_FI) // ends with TOKEN_FI
		return PARSER_UNEXPECTED_TOKEN;

	lexer_pop(lexer);

	return PARSER_OK;

error:
	ast_free(condition);
	return PARSER_UNEXPECTED_TOKEN;
} */

static enum parser_status parse_shell_command(struct ast **res, struct lexer *lexer)
{
	return parse_rule_if(res, lexer);	
}

static enum parser_status parse_rule_if(struct ast **res, struct lexer *lexer)
{
    if (lexer_peek(lexer).type != TOKEN_IF) // must begin with TOKEN_IF
		return PARSER_UNEXPECTED_TOKEN;
	lexer_pop(lexer);
	
	struct ast *command = ast_new(AST_IF);
	*res = command;	
	struct ast *condition;
	if (parse_compound_list(&condition, lexer) != PARSER_OK) // followed by complist
		goto error;

	if (add_child_ast(*res, condition))
		goto error;

	if (lexer_peek(lexer).type != TOKEN_THEN) // followed by TOKEN_THEN
		return PARSER_UNEXPECTED_TOKEN;

	lexer_pop(lexer);

	if (parse_compound_list(&condition, lexer) != PARSER_OK) // compound list
		goto error;

	if (add_child_ast(*res, condition))
		goto error;

	if (parse_else(&condition, lexer) == PARSER_OK) // optional else
	{
		if (add_child_ast(*res, condition))
			goto error;
	}

	if (lexer_peek(lexer).type != TOKEN_FI) // ends with TOKEN_FI
		return PARSER_UNEXPECTED_TOKEN;

	lexer_pop(lexer);

	return PARSER_OK;

    error:
	ast_free(condition);
	return PARSER_UNEXPECTED_TOKEN;	
}

static enum parser_status parse_compound_list(struct ast **res, struct lexer *lexer)
{
	while (lexer_peek(lexer).type == TOKEN_EOL) // {\n}
		lexer_pop(lexer);
	
	if (parse_and_or(res, lexer) != PARSER_OK) // and_or
		return PARSER_UNEXPECTED_TOKEN;

	struct token next = lexer_peek(lexer);
	while (next.type == TOKEN_EOL || next.type == TOKEN_SEMICOLON) // (';' | '\n')
	{
		next = lexer_pop(lexer);
		while (lexer_peek(lexer).type == TOKEN_EOL) // {\n}
			lexer_pop(lexer);

		if (parse_and_or(res, lexer) != PARSER_OK) // and_or
			break;
		next = lexer_peek(lexer);
	}
	if (lexer_peek(lexer).type == TOKEN_SEMICOLON) // [';']
		lexer_pop(lexer);

	while (lexer_peek(lexer).type == TOKEN_EOL) // {\n}
		lexer_pop(lexer);
	return PARSER_OK;

}

static enum parser_status parse_else(struct ast **res, struct lexer *lexer)
{
        struct token next = lexer_peek(lexer);
        if (next.type == TOKEN_ELSE) // else
        {
                next = lexer_pop(lexer);
                return parse_compound_list(res, lexer); // compound list
        }
        else if (next.type == TOKEN_ELIF) // elif
        {
                lexer_pop(lexer);
		
		struct ast *tmp = ast_new(AST_IF);

                if (parse_compound_list(res, lexer) != PARSER_OK) //compound list
			goto error_if_else;

		add_child_ast(tmp, *res);

                if (lexer_peek(lexer).type != TOKEN_THEN) // then
                        goto error_if_else;
                lexer_pop(lexer);

                if (parse_compound_list(res, lexer) != PARSER_OK) // compound list
                        goto error_if_else;

		add_child_ast(tmp, *res);
		
		parse_else(&tmp, lexer); // [else_clause]
		
		*res = tmp;
                return PARSER_OK;
     
error_if_else:
		ast_free(tmp);
		return PARSER_UNEXPECTED_TOKEN;
	}
        return PARSER_UNEXPECTED_TOKEN;
}
