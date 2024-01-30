#include "input.h"

FILE *input_error(FILE *fd)
{
    if (fd)
        fclose(fd);
    fprintf(stderr, "The command line syntax is:");
    fprintf(stderr, " 42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]\n");
    return NULL;
}

FILE *get_input(int argc, char **argv)
{
    if (argc > 3)
        return input_error(NULL);

    FILE *fd = fdopen(0, "r");
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-c"))
        {
            i++;
            if (i == argc)
                return input_error(fd);
            fd = fmemopen(argv[i], strlen(argv[i]), "r");
        }
        else if (!strcmp(argv[i], "-e")) // change the way to handle files
        {
            if (i == argc)
                return input_error(fd);
            i++;
            fd = fopen(argv[i], "r");
        }
        else if (!strcmp(argv[i], "--verbose"))
        {
            if (fd->_fileno != 0)
                return input_error(fd);
            setenv("VERBOSE", "1", 0);
        }
        else if (!strcmp(argv[i], "--pretty-print"))
        {
            if (fd->_fileno != 0)
                return input_error(fd);
            setenv("PRETTY_PRINT", "1", 0);
        }
        else
            return input_error(fd);
    }
    return fd;
}
