#include "dstring.h"

struct Dstring *Dstring_new(void)
{
    return calloc(1, sizeof(struct Dstring));
}

void Dstring_concat(struct Dstring *str, char *s)
{
    for (size_t i = 0; s[i]; i++)
        Dstring_append(str, s[i]);
}

void Dstring_append(struct Dstring *str, char c)
{
    str->value = realloc(str->value, str->size + 1);
    str->value[str->size] = c;
    str->size++;
}

void Dstring_free(struct Dstring *str)
{
    free(str->value);
    free(str);
}
