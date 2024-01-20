#ifndef Dstring_h
#define Dstring_h

#include <stdlib.h>

// a structure defining a string builder
struct Dstring
{
    char *value; // the string
    size_t size; // its size
};

struct Dstring *Dstring_new(void);
void Dstring_append(struct Dstring *str, char c);
void Dstring_free(struct Dstring *str);

#endif /* ! Dstring_h */
