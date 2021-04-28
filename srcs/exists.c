/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exists.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 19:49:56 by ltouret           #+#    #+#             */
/*   Updated: 2021/04/28 19:50:13 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	dir_exists(const char *path, bool follow)
{
	struct stat	stat_buff;

	if (follow)
	{
		if (stat(path, &stat_buff) == -1)
			return (false);
	}
	else
	{
		if (lstat(path, &stat_buff) == -1)
			return (false);
	}
	return (S_ISDIR(stat_buff.st_mode)
		|| (follow && S_ISLNK(stat_buff.st_mode)));
}

bool	file_exists(const char *path)
{
	struct stat	stat_buff;

	return (stat(path, &stat_buff) == 0
		&& S_ISREG(stat_buff.st_mode));
}
