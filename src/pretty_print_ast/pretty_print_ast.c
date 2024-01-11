
#include "print_ast.h"
#include "stdlib.h"

const char *ast_type_to_string(enum ast_type type)
{
    switch (type)
    {
    case AST_IF:
        return "IF";
    case AST_ELIF:
        return "ELIF";
    case AST_WORD:
        return "WORD";
    case AST_COMMAND:
        return "COMMAND";
    default:
        return "UNKNOWN";
    }
}

void generate_dot(struct ast *node, FILE *file)
{
    if (node == NULL)
        return;

    // Print the current node with its label
    fprintf(file, "  \"%p\" [label=\"%s\"];\n", (void *)node,
            ast_type_to_string(node->type));

    // Recursively handle children and draw edges
    for (size_t i = 0; i < node->nbchildren; ++i)
    {
        if (node->children[i])
        {
            // Draw an edge from the current node to the child
            fprintf(file, "  \"%p\" -> \"%p\";\n", (void *)node,
                    (void *)node->children[i]);

            // Recurse into the child node
            generate_dot(node->children[i], file);
        }
    }
}

void pretty_print_ast(struct ast *root)
{
    FILE *file = fopen("ast.dot", "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Start the DOT graph
    fprintf(file, "digraph AST {\n");
    fprintf(file, "  node [shape=box];\n");

    // Generate DOT content
    generate_dot(root, file);

    // End the DOT graph
    fprintf(file, "}\n");
    fclose(file);

    // use Graphviz to convert 'ast.dot' into a visual format
    system("dot -Tpng ast.dot -o ast.png");
}
