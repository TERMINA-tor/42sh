#include "print_ast.h"

void little_print_ast(struct ast *res, int depth)
{
    if (res == NULL)
        return;

    // Add indentation
    for (int i = 0; i < depth; ++i)
    {
        printf("  ");
    }

    switch (res->type)
    {
    case AST_IF:
        printf("if {");
        little_print_ast(res->children[0], depth + 1); // Condition
        printf("}; then {");
        little_print_ast(res->children[1], depth + 1); // Body
        printf("}\n");
        break;

    case AST_ELIF:
        // Iterate and print each child (which should be an AST_IF)
        for (size_t i = 0; i < res->nbchildren; ++i)
        {
            little_print_ast(res->children[i], depth);
            if (i < res->nbchildren - 1)
            {
                printf("elif {");
            }
        }
        break;

    case AST_WORD:
        printf("word \"%s\"\n", "word");
        break;

    case AST_COMMAND:
        printf("command \"");
        // Print command and arguments
        printf("\"\n");
        break;
    }
}
