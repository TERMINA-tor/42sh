#include "../utils/Dstring/dstring.h"

//define a hashmap for assignement

size_t handle_dollar(struct Dstring *dst, char *src)
{
	src++; //skip the $
	//TODO
}

size_t handle_single_quote(struct Dstring *dst, char *src)
{
	size_t len = 0;
	for (size_t i = 1; src[i] && src[i] != '\'', i++) // since src[0] = '
	{
		Dstring_append(dst, src[i]);
		len++;
	}
	if (src[len - 1] == '\'')
		len++;
	return len;
}

size_t handle_double_quotes(struct Dstring *dst, char *src)
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
			Dstring_append(dst, src)
	}
	if (src[len - 1] == '"')
		len++;
	return len;
}

char *expand(const char *str)
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
			Dstring_append(expanded, str[i])
	}
	Dstring_append(expanded, 0);
	char *retval = expanded->value;
	free(expanded);
	return retval;
}