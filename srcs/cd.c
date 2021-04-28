/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 19:46:48 by ltouret           #+#    #+#             */
/*   Updated: 2021/04/28 21:50:53 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error			path_chdir(const char *newpath, const char *oldpath)
{
	t_error	err;

	err = chdir(newpath);
	if (err == OK)
	{
		env_set("OLDPWD", oldpath);
		env_set("PWD", newpath);
	}
	else
		err = ERR_ERRNO;
	return (err);
}

static t_error	exec_cd(t_cd_opts *options)
{
	t_error	err;

	err = OK;
	options->finalpath = ft_strdup(options->path);
	if (!options->finalpath)
		return (ERR_MALLOC);
	err = path_chdir(options->finalpath, options->pwd);
	free(options->finalpath);
	return (err);
}

t_error			builtin_cd(size_t argc, char **argv)
{
	t_cd_opts	options;
	t_error		err;

	err = OK;
	ft_bzero(&options, sizeof(options));
	if (argc > 1)
		return (ERR_TOOMUCH_ARGS);
	options.home = env_get_value("HOME");
	if (argc)
		options.path = *argv;
	else
		options.path = options.home;
	if (!options.path)
		return (OK);
	err = path_pwd(&(options.pwd));
	if (err == OK && options.path)
		err = exec_cd(&options);
	free(options.pwd);
	return (err);
}
