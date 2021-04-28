/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 19:59:06 by ltouret           #+#    #+#             */
/*   Updated: 2021/04/28 19:59:07 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error	builtin_unset(size_t argc, char **argv)
{
	while (argc)
	{
		env_unset(*argv);
		argv++;
		argc--;
	}
	return (OK);
}
