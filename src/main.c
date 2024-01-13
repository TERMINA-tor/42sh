#include <stdio.h>

#include "input/input.h"
#include "parser/parser.h"
#include "eval/evaluation_process.h"

int main(int argc, char **argv)
{
    char *input = get_input(argc, argv);
    if (!input)
	    return 0;

    struct lexer *lexer = lexer_new(input);
    
    if (!lexer)
	    return 0;
    
    // start checking quotes parity
    int missing_quote = 0;
    for (size_t i = 0; lexer->input[i]; i++)
    {
	    if (lexer->input[i] == '\'')
		    missing_quote ^= 1;
    }
    if (missing_quote)
    {
	    fprintf(stderr, "./42sh: missing quote\n");
	    lexer_free(lexer);
	    return 2;
    }
    // end checking quotes parity

    struct ast *res = NULL;
    enum parser_status status = parse_input(&res, lexer);
    
    if (status != PARSER_OK)
    {
	    lexer_free(lexer);
	    ast_free(res);
	    fprintf(stderr, "./42sh: unexpected token\n");
	    return 2;
    }

    int retval = 0;
    if (res)
    {
	retval = evaluate_node(res);
    	ast_free(res);
    }
    free(input);
    lexer_free(lexer);
    if (retval == -1)
	    return 2;
    return 0;
}
