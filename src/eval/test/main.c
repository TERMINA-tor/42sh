#include "../builtins.h"

int main(int argc, char *argv[])
{
    if (argc == 0)
    {
        return 0;
    }
    builtin_echo(argv);
}
