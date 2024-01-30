#include <unistd.h>

#include "input.h"
int main(int argc, char **argv)
{
    FILE *fd = get_input(argc, argv);
    size_t a = 0;
    char x[12] = { 0 };
    do
    {
        a = fread(x, 1, sizeof(char), fd);
        write(STDOUT_FILENO, x, a);
    } while (a > 0);
}
