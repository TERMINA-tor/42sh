#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Création d'une séquence d'instructions : echo "Hello"; echo "World";
    struct ast_sequence *sequence = ast_sequence_init();
    struct ast_cmd *cmd1 = ast_cmd_init();
    cmd1->words = (char *[]){"echo", "Hello", NULL};
    struct ast_cmd *cmd2 = ast_cmd_init();
    cmd2->words = (char *[]){"echo", "World", NULL};

    sequence->commands = (struct ast *[]){(struct ast *)cmd1, (struct ast *)cmd2};
    sequence->num_commands = 2;

    // Création d'une structure if : if (1) { echo "True"; } else { echo "False"; }
    struct ast_if *if_node = ast_if_init();
    if_node->condition = (struct ast*)ast_cmd_init();  // Vous pouvez remplacer par votre propre condition ici
    if_node->then_body = (struct ast*)ast_sequence_init();
    struct ast_cmd *true_cmd = ast_cmd_init();
    true_cmd->words = (char *[]){"echo", "True", NULL};
    ((struct ast_sequence *)if_node->then_body)->commands = (struct ast *[]){(struct ast *)true_cmd};
    ((struct ast_sequence *)if_node->then_body)->num_commands = 1;

    struct ast_cmd *false_cmd = ast_cmd_init();
    false_cmd->words = (char *[]){"echo", "False", NULL};
    if_node->else_body = (struct ast *)false_cmd;

    // Affichage du type des nœuds
    printf("Sequence type: %d\n", sequence->base.type);
    printf("Command 1 type: %d\n", cmd1->base.type);
    printf("Command 2 type: %d\n", cmd2->base.type);
    printf("If type: %d\n", if_node->base.type);

    // Libération de la mémoire
    free_ast((struct ast *)sequence);
    free_ast((struct ast *)if_node);

    return 0;
}
