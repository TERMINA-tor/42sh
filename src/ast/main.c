#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
    // Création d'une séquence d'instructions : echo exemple; echo "World";
    struct ast_sequence *sequence = ast_sequence_init();
    char *exemple = malloc(strlen("hello") + 1);
    strcpy(exemple, "hello");
    char *exemple2 = malloc(strlen("hello") + 1);
    strcpy(exemple2, "hello");
    char *exemple3 = malloc(strlen("hello") + 1);
    strcpy(exemple3, "hello");
    char *exemple4 = malloc(strlen("hello") + 1);
    strcpy(exemple, "hello");
    struct ast_cmd *cmd1 = ast_cmd_init();
    cmd1 = (struct ast_cmd*)ast_cmd_word_add((struct ast*)cmd1, exemple);

    struct ast_cmd *cmd2 = ast_cmd_init();
    cmd2 = (struct ast_cmd*)ast_cmd_word_add((struct ast*)cmd2, exemple2);

    sequence = (struct ast_sequence *)ast_sequence_add((struct ast *)sequence, (struct ast *)cmd1);
    sequence = (struct ast_sequence *)ast_sequence_add((struct ast *)sequence, (struct ast *)cmd2);

    // Création d'une condition IF : if (condition) then_body else else_body
    struct ast_if *if_statement = ast_if_init();

    struct ast_cmd *if_cmd = ast_cmd_init();
    if_cmd = (struct ast_cmd*)ast_cmd_word_add((struct ast*)if_cmd, exemple3);

    struct ast_cmd *else_cmd = ast_cmd_init();
    else_cmd = (struct ast_cmd*)ast_cmd_word_add((struct ast*)else_cmd, exemple4);

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
