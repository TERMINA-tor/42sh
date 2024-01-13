#include "input.h"

char *input_error(char *input, FILE *fd)
{
    free(input);
    if (fd)
        fclose(fd);
    fprintf(stderr, "The command line syntax is:");
    fprintf(stderr, " 42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]\n");
    return NULL;
}

char *get_input(int argc, char **argv)
{
    if (argc > 3)
        return input_error(NULL, NULL);
    char *input = 0;
    size_t input_size = 0;
    FILE *fd = fdopen(0, "r");
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-c"))
        {
            i++;
            if (i == argc)
                return input_error(NULL, fd);
            fd = fmemopen(argv[i], strlen(argv[i]), "r");
        }
        else if (!strcmp(argv[i], "-e"))
        {
            if (i == argc)
                return input_error(NULL, fd);
            i++;
            fd = fopen(argv[i], "r");
        }
        else if (!strcmp(argv[i], "--verbose"))
        {
            if (fd->_fileno != 0)
                return input_error(NULL, fd);
            setenv("VERBOSE", "1", 0);
        }
        else if (!strcmp(argv[i], "--pretty-print"))
        {
            if (fd->_fileno != 0)
                return input_error(NULL, fd);
            setenv("PRETTY_PRINT", "1", 0);
        }
        else
            return input_error(NULL, fd);
    }

    char buffer[128];
    size_t a = 1;

    do
    {
        a = fread(buffer, 1, 128, fd);
        input_size += a;
        input = realloc(input, input_size * sizeof(char));
        memcpy(input + input_size - a, buffer, a);
    } while (a > 0);

    input = realloc(input, input_size + 1);
    input[input_size] = 0;
    fclose(fd);
    return input;
}
