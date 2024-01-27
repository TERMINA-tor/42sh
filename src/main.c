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
	    return -1;
    struct ast *res = NULL;
    printf("%d\n", parse_input(&res, lexer) == PARSER_OK);
    evaluate_node(res);
    if (res)
	    free_ast(res);
    if (lexer)
	    lexer_free(lexer);
    return 0;
}
