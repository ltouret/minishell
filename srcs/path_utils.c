/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 19:58:28 by ltouret           #+#    #+#             */
/*   Updated: 2021/04/28 19:58:42 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_error	path_trailslash(char **str)
{
	char	*str2;
	size_t	str_len;

	str_len = ft_strlen(*str);
	if ((*str)[str_len - 1] == '/')
		return (OK);
	str2 = malloc((str_len + 2) * sizeof(char));
	if (!str2)
		return (ERR_MALLOC);
	ft_memcpy(str2, *str, str_len);
	str2[str_len] = '/';
	str2[str_len + 1] = '\0';
	free(*str);
	*str = str2;
	return (OK);
}

t_error		path_pwd(char **str)
{
	char	buff[PATH_MAX];
	t_error	err;

	*str = ft_strdup(getcwd(buff, PATH_MAX));
	if (!*str)
		return (ERR_MALLOC);
	err = path_trailslash(str);
	if (err != OK)
		return (err);
	return (OK);
}
