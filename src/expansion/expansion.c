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

struct Dlist *Dlist_init(void)
{
    return calloc(1, sizeof(struct Dlist));
}

static void Dlist_append(struct Dlist *list, char *s)
{
    if (!s)
        return;
    struct Dstring *str = Dstring_new();
    for (size_t i = 0; s[i]; i++)
        Dstring_append(str, s[i]);
    Dstring_append(str, 0);
    list->list = realloc(list->list, (list->size + 1) * sizeof(char *));
    list->list[list->size] = str->value;
    list->size += 1;
    free(str);
}

void Dlist_free(struct Dlist *list)
{
    for (size_t i = 0; i < list->size; i++)
        free(list->list[i]);
    free(list->list);
    free(list);
}

static int is_valid(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9') || c == '_' || c == '@' || c == '#'
        || c == '*' || c == '?';
}

static void append_int(struct Dstring *dst, int value)
{
    if (!value)
    {
        Dstring_append(dst, '0');
        return;
    }
    int tmp = value;
    int len = 0;
    while (tmp != 0)
    {
        len++;
        tmp /= 10;
    }
    char *conv = calloc(len + 1, sizeof(char));
    while (value)
    {
        conv[len - 1] = (value % 10) + '0';
        value /= 10;
        len--;
    }
    Dstring_concat(dst, conv);
    free(conv);
}

static void search_dictionary(struct Dlist *list, struct Dstring *arg,
                              struct Dstring *dst)
{
    struct dictionary *head = g_cache.dictionary;
    while (head)
    {
        if (!strcmp(head->name, arg->value))
        {
            Dstring_concat(dst, head->name);
            return;
        }
        head = head->next;
    }
    Dstring_append(dst, 0);
    Dlist_append(list, dst->value);
    free(dst->value);
    dst->value = NULL;
    dst->size = 0;
}

int get_nth(struct Dstring *dst, char *arg, int in_brackets)
{
    if (!arg)
        return 0;
    int index = 0;
    if (in_brackets)
    {
        for (size_t i = 0; arg[i]; i++)
        {
            if (arg[i] < '0' || arg[i] > '9')
                return 0;
            index *= 10;
            index += arg[i] - '0';
        }
    }
    else
    {
        if (*arg > '9' || *arg < '0')
            return 0;
        index = *arg - '0';
    }

    if (index && index <= g_cache.argc) // $0 is forbidden
        Dstring_concat(dst, g_cache.argv[index - 1]);
    return 1;
}

// try to find static variabled (not environment variables)
// if the variable is not $@, the value is appended to dst
// at the end of the dunction dsts value is added to list
static void search_cache(struct Dstring *arg, struct Dlist *list,
                         struct Dstring *dst, int in_brackets)
{
    if (!strcmp(arg->value, "*"))
    {
        for (int i = 0; i < g_cache.argc; i++)
        {
            Dstring_concat(dst, g_cache.argv[i]);
            if (i + 1 < g_cache.argc)
                Dstring_append(dst, ' ');
        }
    }
    else if (!strcmp(arg->value, "@"))
    {
        if (g_cache.argc == 0)
            return;
        for (int i = 0; i < g_cache.argc - 1; i++)
        {
            Dstring_concat(dst, g_cache.argv[i]);
            Dstring_append(dst, 0);
            Dlist_append(list, dst->value);
            free(dst->value);
            dst->value = NULL;
            dst->size = 0;
        }
        Dstring_concat(dst, g_cache.argv[g_cache.argc - 1]);
        return;
    }
    else if (!(strcmp(arg->value, "$")))
    {
        append_int(dst, getpid());
    }
    else if (!strcmp(arg->value, "?"))
    {
        append_int(dst, g_cache.least_retval);
    }
    else if (!strcmp(arg->value, "#"))
    {
        append_int(dst, g_cache.argc);
    }
    else if (!strcmp(arg->value, "RANDOM"))
    {
        append_int(dst, rand());
    }
    else if (!strcmp(arg->value, "UID"))
    {
        append_int(dst, getuid());
    }
    else
    {
        if (!get_nth(dst, arg->value, in_brackets))
            search_dictionary(list, arg, dst);
    }
    // Dlist_append(list, dst->value);
}

static size_t handle_dollar(struct Dlist *list, char *src, struct Dstring *dst)
{
    char *dupe = src;
    int is_embeded = 0;
    dupe++;
    int expand_dollar = 1;
    struct Dstring *parameter = Dstring_new();
    if (*dupe == '{')
    {
        is_embeded = 1;
        dupe++;
    }
    else if (*dupe == '$')
    {
        Dstring_append(parameter, '$');
        dupe++;
    }

    while (*dupe && is_valid(*dupe))
    {
        Dstring_append(parameter, *dupe);
        dupe++;
    }
    if (!parameter->size)
    {
	    expand_dollar = 0;
	    Dstring_append(parameter, '$');
	    while (*dupe && *dupe != ' ')
	    {
		    Dstring_append(parameter, *dupe);
		    dupe++;
	    }
	    Dstring_append(parameter, *dupe);
    }
    if (*dupe == '}')
    {
        if (is_embeded)
            is_embeded -= 1;
        else
            dupe--;
    }
    else if (*dupe == '$')
        dupe--; 
    Dstring_append(parameter, 0);
    char *tmp = getenv(parameter->value);
    if (tmp && expand_dollar)
        Dstring_concat(dst, tmp);
    else if (expand_dollar)
        search_cache(parameter, list, dst, *dupe == '}');
    else if (!expand_dollar)
	    Dstring_concat(dst, parameter->value);

    Dstring_free(parameter);
    if (is_embeded)
        return -1;
    size_t retval = dupe - src;
    if (retval && *dupe == 0) // only performed if src' end is reached
        retval--;
    return retval;
}

static size_t handle_single_quote(struct Dstring *dst, char *src)
{
    size_t len = 0;
    for (size_t i = 1; src[i] && src[i] != '\''; i++) // since src[0] = '
    {
        Dstring_append(dst, src[i]);
        len++;
    }
    if (src[len] == 0)
        return 42; // error
    len++;
    return len;
}

static void append_local(struct Dlist *list, struct Dstring *str)
{
    Dstring_append(str, 0);
    Dlist_append(list, str->value);
    Dstring_free(str);
}

static size_t handle_double_quote(struct Dlist *list, char *src,
                                  struct Dstring *dst)
{
    size_t len = 1;
    int is_escaped = 0;
    for (size_t i = 1; src[i] && (src[i] != '"' || (src[i] == '"' && is_escaped)); i++)
    {
        if (src[i] == '\\' && !is_escaped)
        {
            is_escaped = 1;
            len++;
        }
        else if (!is_escaped && src[i] == '$')
        {
            size_t offset = handle_dollar(list, src + i, dst);
            len += offset;
            i += offset;
        }
        else
        {
            is_escaped = 0;
            len++;
            Dstring_append(dst, src[i]);
        }
    }
    if (src[len] == 0 && src[len - 1] != '"')
    {
        fprintf(stderr, "./42sh: missing quote\n");
        return -1; // error
    }
    return len;
}

int expand(struct Dlist *list, char *str)
{
    struct Dstring *expanded = Dstring_new();
    int is_escaped = 0;
    int aux;
    for (size_t i = 0; str[i] != 0; i++)
    {
        if (str[i] == '\\' && !is_escaped)
        {
            is_escaped = 1;
            continue;
        }
        else if (str[i] == '\'' && !is_escaped)
        {
            aux = handle_single_quote(expanded, str + i);
            if (aux == -1)
                goto error;
            i += aux;
        }
        else if (str[i] == '"' && !is_escaped)
        {
            aux = handle_double_quote(list, str + i, expanded);
            if (aux == -1)
                goto error;
            i += aux;
        }
        else if (str[i] == '$' && !is_escaped)
        {
            aux = handle_dollar(list, str + i,
                               expanded); // must handle error cases here TODO
            if (aux == -1)
                goto error;
            i += aux;
        }
        else
        {
            is_escaped = 0;
            Dstring_append(expanded, str[i]);
        }
    }
    if (expanded->size)
        append_local(list, expanded);
    else
        free(expanded);
    return 0;
error:
    if (expanded->size)
        append_local(list, expanded);
    else
        free(expanded);
    return -1;
}
