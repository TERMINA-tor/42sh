#include <stdio.h>

#include "../input/input.h"
#include "parser.h"

int main(int argc, char **argv)
{
    char *input = get_input(argc, argv);
    struct lexer *lexer = lexer_new(input);
    struct ast *res = NULL;
    enum parser_status result = parse_input(&res, lexer);

    // print_ast(res);
    free(input);
    lexer_free(lexer);

    if (!res)
        return 1;
    ast_free(res);
    return result;
}
