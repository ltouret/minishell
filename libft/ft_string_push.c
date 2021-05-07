/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string_push.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 17:39:13 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/07 23:48:57 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

t_string	*ft_string_push(t_string *string, char *to_append)
{
	size_t	to_append_len;
	char	*tmp;

	if (!to_append || !string)
		return (NULL);
	to_append_len = ft_strlen(to_append);
	if (string->capacity < string->len + to_append_len + 1)
	{
		tmp = malloc(sizeof(char) * (string->len + to_append_len + 1));
		if (!tmp)
			return (NULL);
		string->capacity = string->len + to_append_len + 1;
		ft_strlcpy(tmp, string->str, string->len + to_append_len + 1);
		ft_strlcpy(tmp + string->len, to_append, to_append_len + 1);
		free(string->str);
		string->str = tmp;
	}
	else
	{
		ft_strlcpy(string->str + string->len, to_append,
				string->capacity - string->len);
	}
	string->len += to_append_len;
	return (string);
}

t_string	*ft_string_push_char(t_string *string, char to_append)
{
	char	*tmp;

	if (string->capacity < string->len + 2)
	{
		tmp = malloc(sizeof(char) * (string->len + 2));
		if (!tmp)
			return (NULL);
		string->capacity = string->len + 2;
		ft_strlcpy(tmp, string->str, string->len + 2);
		tmp[string->len] = to_append;
		tmp[string->len + 1] = '\0';
		free(string->str);
		string->str = tmp;
	}
	else
	{
		string->str[string->len] = to_append;
		string->str[string->len + 1] = '\0';
	}
	string->len += 1;
	return (string);
}
