//header

#include "minishell.h"

void	pp_realloc(t_pp_state *st, t_string *str, size_t capacity)
{
	char	*old_str;

	old_str = str->str;
	str->str = malloc(sizeof(char) * capacity);
	if (!str->str)
		panic(ERR_MALLOC);
	ft_memcpy(str->str, old_str, str->len + 1);
	str->capacity = capacity;
	st->cursor += str->str - old_str;
	st->anchor += str->str - old_str;
	free(old_str);
}

void	pp_replace(t_pp_state *st, ssize_t oldlen, char *newword, t_string *str)
{
	ssize_t	newlen;
	size_t	totallen;
	char	*at;

	newlen = ft_strlen(newword);
	totallen = str->len - oldlen + newlen + 1;
	if (str->capacity < totallen)
		pp_realloc(st, str, totallen);
	at = st->cursor;
	ft_memmove(at + newlen, at + oldlen,
		str->len - oldlen - (at - str->str) + 1);
	ft_memcpy(at, newword, newlen);
	str->len -= oldlen - newlen;
	st->cursor += newlen - 1;
	//printf("at %s cursor %s str %s\n", at, st->cursor, str->str);
}

void	pp_strip(t_pp_state *st, ssize_t striplen, t_string *str)
{
	char	*at;

	if (!striplen)
		return ;
	at = st->cursor;
	//printf("strip %s\n", at);
	ft_memmove(at, at + striplen, str->len - striplen - (at - str->str) + 1);
	//printf("strip %s\n", at);
	str->len -= striplen;
	st->cursor--;
}
