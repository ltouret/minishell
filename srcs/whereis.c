/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whereis.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 19:59:10 by ltouret           #+#    #+#             */
/*   Updated: 2021/04/28 21:30:28 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*test_paths(char **paths, const char *filepath)
{
	char	*pathname;

	while (*paths)
	{
		pathname = path_join(*paths, filepath);
		if (!pathname)
			panic(ERR_MALLOC);
		if (file_exists(pathname))
			return (pathname);
		free(pathname);
		paths++;
	}
	return (NULL);
}

char		*whereis(const char *filepath)
{
	char	*env_path;
	char	**paths;
	char	**paths_cpy;
	char	*pathname;

	env_path = env_get_value("PATH");
	if (*filepath == '/' || *filepath == '.')
		return (ft_strdup(filepath));
	else if (env_path)
	{
		paths = ft_split(env_path, ":");
		if (!paths)
			panic(ERR_MALLOC);
		paths_cpy = paths;
		pathname = test_paths(paths, filepath);
		while (*paths_cpy)
			free(*(paths_cpy++));
		free(paths);
		return (pathname);
	}
	return (NULL);
}
