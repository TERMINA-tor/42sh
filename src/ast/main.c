#include "ast.h"
#include <stdio.h>

int main() {
    // Création d'une séquence d'instructions : echo "Hello"; echo "World";
    struct ast_sequence *sequence = ast_sequence_init();

    struct ast_cmd *cmd1 = ast_cmd_init();
    cmd1 = (struct ast_cmd*)ast_cmd_word_add((struct ast*)cmd1, "hello");

    struct ast_cmd *cmd2 = ast_cmd_init();
    cmd2 = (struct ast_cmd*)ast_cmd_word_add((struct ast*)cmd2, "hello");

    sequence = (struct ast_sequence *)ast_sequence_add((struct ast *)sequence, (struct ast *)cmd1);
    sequence = (struct ast_sequence *)ast_sequence_add((struct ast *)sequence, (struct ast *)cmd2);

    // Création d'une condition IF : if (condition) then_body else else_body
    struct ast_if *if_statement = ast_if_init();

    struct ast_cmd *if_cmd = ast_cmd_init();
    if_cmd = (struct ast_cmd*)ast_cmd_word_add((struct ast*)if_cmd, "hello");

    struct ast_cmd *else_cmd = ast_cmd_init();
    else_cmd = (struct ast_cmd*)ast_cmd_word_add((struct ast*)else_cmd, "hello");

    if_statement = (struct ast_if *)ast_if_condition_add((struct ast *)if_statement, (struct ast *)if_cmd);
    if_statement = (struct ast_if *)ast_if_then_add((struct ast *)if_statement, (struct ast *)else_cmd);

    // Ajout de la séquence et de l'instruction IF à une nouvelle séquence
    struct ast_sequence *main_sequence = ast_sequence_init();
    main_sequence = (struct ast_sequence *)ast_sequence_add((struct ast *)main_sequence, (struct ast *)sequence);
    main_sequence = (struct ast_sequence *)ast_sequence_add((struct ast *)main_sequence, (struct ast *)if_statement);

    // Affichage du type des nœuds
    printf("Sequence type: %d\n", main_sequence->base.type);

    // Libération de la mémoire
    free_ast((struct ast *)main_sequence);

    return 0;
}
