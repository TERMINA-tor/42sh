#ifndef EVALUATION_PROCESS_H
#define EVALUATION_PROCESS_H

#include "../expansion/expansion.h"

int evaluate_ast(struct ast *node);
void set_loop_break_flag(int);
void set_loop_continue_flag(int);
void clean_ast();

#endif /* !EVALUATION_PROCESS_H */
