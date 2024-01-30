#ifndef EXPANSION_H
#define EXPANSION_H

#include "../ast/ast.h"
#include "../utils/Dstring/dstring.h"

// a structure used to store specific information
// will be allocated on the stack
struct cache
{
    int argc; // number of given args
    char **argv; // list of given args
    struct dictionary *dictionary; // stored variables
};

// a linked listed used to store variables
struct dictionary
{
    char *name; // the name of the variable
    char *value; // its value

    struct dictionary *prev; // the previous variable
    struct dictionary *next; // the next variable
};

extern struct cache g_cache; // the cache defined in main

// simply initializes a dictionary structure
struct dictionary *init_dictionary(char *name, char *value);

char *expand(char *str);

// add a variable in the dictionary
#endif /* ! EXPANSION_H */
