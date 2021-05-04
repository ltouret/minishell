/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 19:50:16 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/03 00:14:42 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error	builtin_exit(size_t argc, char **argv)
{
	int status;

	if (argc > 1)
		return (ERR_TOOMUCH_ARGS);
	if (argc == 1)
		status = ft_atoi(*argv);
	free_data(get_data());
	env_shutdown();
	if (argc == 1)
		exit((unsigned char)status);
	else
		exit(0);
	return (OK);
}
