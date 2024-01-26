#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdio.h>

// The 'true' built-in function
int builtin_true(void);

// The 'false' built-in function
int builtin_false(void);

// The 'echo' built-in function
int builtin_echo(char **argv, size_t nbr_args);

#endif // BUILTINS_H
