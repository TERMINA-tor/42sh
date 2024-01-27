#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"

#define true 1
#define false 0

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

void print_args(char **argv, int interpret_escapes);
// The 'echo' built-in function
int builtin_echo(char **argv, size_t nbr_args)
{
    int newline = true; // By default, echo prints a newline at the end
    int interpret_escapes =
        false; // By default, do not interpret special characters

    // Skip the command name
    size_t i = 1;

    // Process options
    while (i < nbr_args && *argv[i] == '-')
    {
        char *option = argv[i] + 1;
        while (*option != 0)
        {
            switch (*option)
            {
            case 'n':
                newline = false;
                break;
            case 'e':
                interpret_escapes = true;
                break;
            case 'E':
                interpret_escapes = false;
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
    print_args(argv, interpret_escapes);

    if (newline)
    {
        putchar('\n');
    }

    fflush(stdout); // Flush the output
    return 0;
}

void print_args(char **argv, int interpret_escapes)
{
    while (*argv)
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
        if (*(argv + 1))
        {
            putchar(' '); // Separate arguments with a space
        }
        argv++;
    }
}
