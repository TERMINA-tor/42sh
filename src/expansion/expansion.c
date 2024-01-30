#include "expansion.h"

struct dictionary *init_dictionary(char *name, char *value)
{
    struct dictionary *dict = calloc(1, sizeof(struct dictionary));
    if (!dict)
        return NULL;
    dict->name = name;
    dict->value = value;

    return dict;
}

static size_t handle_dollar(struct Dstring *dst, char *src)
{
    struct Dstring *parameter = Dstring_new();
    char *dupe = src;
    int is_embeded = 0;
    dupe++;

    if (*dupe == '{')
    {
        is_embeded = 1;
        dupe++;
    }
    while (*dupe && *dupe != '$')
    {
        if (*dupe == '}')
            dupe--;
        Dstring_append(parameter, *dupe);
    }
    Dstring_append(parameter, 0);
    Dstring_concat(dst, (char *)getenv(parameter->value));
    Dstring_free(parameter);
    if (is_embeded)
        return -1;
    return dupe - src;
}

static size_t handle_single_quote(struct Dstring *dst, char *src)
{
    size_t len = 0;
    for (size_t i = 1; src[i] && src[i] != '\''; i++) // since src[0] = '
    {
        Dstring_append(dst, src[i]);
        len++;
    }
    if (src[len - 1] == '\'')
        len++;
    return len;
}

static size_t handle_double_quote(struct Dstring *dst, char *src)
{
    size_t len = 0;
    for (size_t i = 1; src[i] && src[i] != '"'; i++)
    {
        if (src[i] == '$')
        {
            size_t offset = handle_dollar(dst, src + i);
            len += offset;
            i += offset;
        }
        else
            Dstring_append(dst, src[i]);
    }
    if (src[len - 1] == '"')
        len++;
    return len;
}

char *expand(char *str)
{
    struct Dstring *expanded = Dstring_new();
    for (size_t i = 0; str[i] != 0; i++)
    {
        if (str[i] == '\'')
            i += handle_single_quote(expanded, str + i);
        else if (str[i] == '"')
            i += handle_double_quote(expanded, str + i);
        else if (str[i] == '$')
            i += handle_dollar(expanded, str + i);
        else
            Dstring_append(expanded, str[i]);
    }
    Dstring_append(expanded, 0);
    char *retval = expanded->value;
    free(expanded);
    return retval;
}
