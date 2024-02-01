#include "dstring.h"

struct Dstring *Dstring_new(void)
{
    return calloc(1, sizeof(struct Dstring));
}

void Dstring_concat(struct Dstring *str, char *s)
{
    if (!s)
	    return;
    for (size_t i = 0; s[i]; i++)
        Dstring_append(str, s[i]);
}

void Dstring_append(struct Dstring *str, char c)
{
    str->value = realloc(str->value, str->size + 1);
    str->value[str->size] = c;
    str->size++;
}

void Dstring_concat_string(struct Dstring *str, struct Dstring* str2)
{
    for (size_t i = 0; i < str2->size; i++)
        Dstring_append(str, str2->value[i]);
}

void Dstring_free(struct Dstring *str)
{
    free(str->value);
    free(str);
}
