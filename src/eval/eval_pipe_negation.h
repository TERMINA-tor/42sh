#ifndef EVAL_PIPE_NEGATION_H
#define EVAL_PIPE_NEGATION_H

#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "evaluation_process.h"

#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

int eval_pipeline(struct ast *node);
// int eval_negation(struct ast *node);

#endif // EVAL_PIPE_NEGATION_H