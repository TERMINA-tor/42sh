#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum input_status
{
    INPUT_OK,
    INPUT_ERROR
};

char *input_error(char *input, FILE *fd);
char *get_input(int argc, char **argv);

#endif /* INPUT_H */
