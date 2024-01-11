#ifndef PARSER_H
#define PARSER_H

enum parser_status
{
	PARSER_OK,
	PARSER_UNEXPECTED_TOKEN,
};

/**
 ** \brief parse input rule
 * input =
 * 	list '\n'
 *    | list EOF
 *    | '\n'
 *    | EOF
 */
enum parser_status parse_input(struct ast **res, struct lexer *lexer);

enum parser_status parse_list(struct ast **res, struct lexer *lexer);

enum parser_status parse_and_or(struct ast **res, struct lexer *lexer);

enum parser_status parse_command(struct ast **res, struct lexer *lexer);

enum parser_status parse_simple_command(struct ast **res, struct lexer *lexer);

enum parser_status parse_element(struct ast **res, struct lexer *lexer);

#endif /* ! PARSER_H */
