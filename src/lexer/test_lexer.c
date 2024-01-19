#include "lexer.h"
#include "../input/input.h"

void lut(struct token t)
{
    const struct lookuptable table[] = { { TOKEN_EOF, "" },
                                         { TOKEN_EOL, "\n" },
                                         { TOKEN_IF, "if" },
                                         { TOKEN_ELSE, "else" },
                                         { TOKEN_THEN, "then" },
                                         { TOKEN_ELIF, "elif" },
                                         { TOKEN_FI, "fi" },
                                         { TOKEN_WHILE, "while" },
                                         { TOKEN_UNTIL, "until" },
                                         { TOKEN_FOR, "for" },
                                         { TOKEN_DO, "do" },
                                         { TOKEN_DONE, "done" },
                                         { TOKEN_AND, "&&" },
                                         { TOKEN_OR, "||" },
                                         { TOKEN_REDIRECT_INPUT, "<" },
                                         { TOKEN_REDIRECT_OUTPUT, "<" },
                                         { TOKEN_APPEND_OUTPUT, ">>" },
                                         { TOKEN_AMPREDIR_OUTPUT, ">&" },
                                         { TOKEN_AMPREDIR_INPUT, "<&" },
                                         { TOKEN_FORCE_OUTPUT_REDIR, ">|" } };
    size_t table_length = sizeof(table) / sizeof(struct lookuptable);
    for (size_t i = 0; i < table_length; i++)
    {
        if (table[i].token_type == t.type)
	{
            printf("%s\n", table[i].value);
	    return;
	}
    }
    printf("%s\n", t.value);
}


int main(int argc, char **argv)
{
	FILE *fd = get_input(argc, argv);
	struct lexer *lexer = init_lexer(fd);
	struct token t;
	do
	{
		t = lexer_pop(lexer);
		lut(t);
	} while (t.type != TOKEN_EOF);
	lexer_free(lexer);
}
