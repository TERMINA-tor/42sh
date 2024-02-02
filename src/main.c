#include "ast/ast.h"
#include "eval/evaluation_process.h"
#include "expansion/expansion.h"
#include "input/input.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "utils/Dstring/dstring.h"

struct cache g_cache;

void init_cache(int argc, char **argv)
{
    g_cache.argc = argc;
    g_cache.argv = argv;
    g_cache.dictionary = NULL;
    g_cache.least_retval = 0;
}

int main(int argc, char **argv)
{
    init_cache(argc, argv);
    FILE *fd = get_input(argc, argv);
    if (!fd)
        return -1;
    struct lexer *lexer = init_lexer(fd);
    if (!lexer)
        goto error_1;

    struct ast *res = NULL;
    int retval = 0;
    while (lexer_peek(lexer).type != TOKEN_EOF)
    {
        while (lexer_peek(lexer).type == TOKEN_EOL)
            lexer_pop(lexer);
        if (lexer_peek(lexer).type == TOKEN_EOF)
            break;
        enum parser_status status = parse_input(&res, lexer);
        if (status == PARSER_UNEXPECTED_TOKEN)
            goto error_2;
        retval = evaluate_ast(res);
        if (res)
        {
            free_ast(res);
            res = NULL;
        }
        g_cache.least_retval = retval;
        if (retval != 0 && retval != 1)
            break;
    }
    if (lexer)
        lexer_free(lexer);
    return g_cache.least_retval;

error_1:
    fprintf(stderr, "./42sh: failed to allocate memory\n");
    return 127;

error_2:
    if (lexer)
        lexer_free(lexer);
    if (res)
        free_ast(res);
    fprintf(stderr, "./42sh: parser: unexpected_token\n");
    return 2;
}
