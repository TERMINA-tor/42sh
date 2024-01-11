#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"

char *buffer_path = "/tmp/test_output.txt";

// A helper function to compare the output of echo with an expected string.
void test_echo_output(char **argv, const char *expected, int expectNewline)
{
    // Redirect stdout to a buffer
    freopen(buffer_path, "w+", stdout);

    builtin_echo(argv);
    fflush(stdout);

    // Reset stdout to its original state
    freopen("/dev/tty", "a", stdout);

    // Open the temporary file for reading
    FILE *f = fopen(buffer_path, "r");
    if (!f)
    {
        perror("fopen failed");
        exit(EXIT_FAILURE);
    }

    // Read the contents of the file into a dynamically allocated buffer
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buffer = malloc(length + 1);
    if (!buffer)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, length, f);
    buffer[length] = '\0';
    fclose(f);

    printf("expected:\n %s", expected);
    printf("result:\n %s\n", buffer);
    assert(strcmp(buffer, expected) == 0);

    free(buffer);
}

int main()
{
    // Test builtin_true
    assert(builtin_true() == 0);

    // Test builtin_false
    assert(builtin_false() != 0);

    // Test builtin_echo with no options
    char *argv1[] = { "echo", "hello", "world", NULL };
    test_echo_output(argv1, "hello world\n", 1);

    // Test builtin_echo with -n option
    char *argv2[] = { "echo", "-n", "hello", "world", NULL };
    test_echo_output(argv2, "hello world", 0);
    char *argv2bis[] = { "echo", "-n", "No newline at the end", NULL };
    test_echo_output(argv2bis, "No newline at the end", 0);

    // Test builtin_echo with -e option
    char *argv3[] = { "echo", "-e", "hello\nworld", NULL };
    test_echo_output(argv3, "hello\nworld\n", 1);
    char *argv3bis[] = { "echo", "-e", "hello", "world", NULL };
    test_echo_output(argv3bis, "hello world\n", 1);

    // Test builtin_echo with -E option
    char *argv4[] = { "echo", "-E", "hello\\nworld", NULL };
    test_echo_output(argv4, "hello\\nworld\n", 1);

    // Combo testion with multiple options
    char *argv6[] = { "echo", "-ne",
                      "This is a newline\\nThis is a tab\\tEnd of message",
                      NULL };
    test_echo_output(argv6, "This is a newline\nThis is a tab\tEnd of message",
                     0);

    char *argv7[] = { "echo", "-nE", "Escapes \\n \\t are not interpreted",
                      NULL };
    test_echo_output(argv7, "Escapes \\n \\t are not interpreted", 0);

    remove(buffer_path); // Clean up the temporary file
    printf("All tests passed.\n");
    return 0;
}
