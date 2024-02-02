#ifndef EVAL_REDIRECTIONS_H
#define EVAL_REDIRECTIONS_H

#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "evaluation_process.h"

int eval_redirections(struct ast *node);

#endif /* ! EVAL_REDIRECTIONS_H */
