#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"

// The 'true' built-in function
int builtin_true(void)
{
    return 0;
}

// The 'false' built-in function
int builtin_false(void)
{
    return 1;
}

void print_args(char **argv, int interpret_escapes, size_t len);
// The 'echo' built-in function
int builtin_echo(char **argv, size_t nbr_args)
{
    int newline = 1; // By default, echo prints a newline at the end
    int interpret_escapes =0; // By default, do not interpret special characters

    // Skip the command name
    size_t i = 1;

    // Process options
    while (i < (nbr_args - 1) && *argv[i] == '-')
    {
        char *option = argv[i] + 1;
        while (*option)
        {
            switch (*option)
            {
            case 'n':
                newline = 0;
                break;
            case 'e':
                interpret_escapes = 1;
                break;
            case 'E':
                interpret_escapes = 0;
                break;
            default:
                fprintf(stderr, "Unknown option: -%c\n", *option);
                return 1;
            }
            option++;
        }
        i++;
    }

    // Print arguments
    print_args(argv, interpret_escapes, nbr_args - 1);

    if (newline)
    {
        putchar('\n');
    }

    fflush(stdout); // Flush the output
    return 0;
}

void print_args(char **argv, int interpret_escapes, size_t len)
{
    size_t i = 0;
    while (i < len && *argv)
    {
        if (interpret_escapes)
        {
            char *arg = *argv;
            while (*arg)
            {
                switch (*arg)
                {
                case '\\': // Escape sequence
                    arg++;
                    switch (*arg)
                    {
                    case 'n':
                        putchar('\n');
                        break;
                    case 't':
                        putchar('\t');
                        break;
                    case '\\':
                        putchar('\\');
                        break;
                    default:
                        putchar('\\');
                        putchar(*arg);
                        break;
                    }
                    break;
                default:
                    putchar(*arg);
                }
                arg++;
            }
        }
        else
        {
            printf("%s", *argv);
        }
        if ((i + 1) < len && *(argv + 1))
        {
            putchar(' '); // Separate arguments with a space
        }
        argv++;
        i++;
    }
}
