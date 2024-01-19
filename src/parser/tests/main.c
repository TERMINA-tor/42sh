#include <stdio.h>

#include "../../input/input.h"
#include "../../lexer/lexer.h"
#include "../parser.h"

int main(int argc, char **argv)
{
    char *input = get_input(argc, argv);
    if (!input)
	    return 0;

    struct lexer *lexer = lexer_new(input);
    
    if (!lexer)
	    return 0;
    
    enum parser_status status = parse_input(lexer);
    if (status != PARSER_OK)
        printf("Error\n");
    else
        printf("OK\n");
}