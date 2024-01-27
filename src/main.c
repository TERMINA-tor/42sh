#include "ast/ast.h"
#include "eval/evaluation_process.h"
#include "expansion/expansion.h"
#include "input/input.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "utils/Dstring/dstring.h"

int main(int argc, char **argv)
{
    FILE *fd = get_input(argc, argv);
    if (!fd)
	    return -1;
    struct lexer *lexer = init_lexer(fd);
    if (!lexer)
	    goto error_1;

    struct ast *res = NULL;
    enum parser_status status = parse_input(&res, lexer);
    if (status == PARSER_UNEXPECTED_TOKEN)
	    goto error_2;
    int retval = evaluate_node(res);
    if (res)
	    free_ast(res);
    if (lexer)
	    lexer_free(lexer);
    return retval;

error_1:
    fprintf(stderr, "./42sh: failed to allocate memory\n");
    return 127;

error_2:
    fprintf(stderr, "./42sh: parser: unexpected_token\n");
    return 2;
}
