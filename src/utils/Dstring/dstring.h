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
void Dstring_concat(struct Dstring *str, char *s);
void Dstring_append(struct Dstring *str, char c);
void Dstring_concat_string(struct Dstring *str, struct Dstring *str2);
void Dstring_free(struct Dstring *str);

#endif /* ! Dstring_h */
