#include <stdio.h>

#include "../../input/input.h"
#include "../../lexer/lexer.h"
#include "../parser.h"

int main(int argc, char **argv)
{
    FILE *input = get_input(argc, argv);
    if (!input)
	    return 0;

    struct lexer *lexer = init_lexer(input);
    
    if (!lexer)
	    return 0;
    
    enum parser_status status = parse_input(lexer);
    if (status != PARSER_OK)
        printf("error\n");
    else
        printf("OK\n");

    lexer_free(lexer);
}