/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 19:50:52 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/06 17:25:04 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void	sig_int(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		if (*print_prompt())
		{
			//*get_counter() = 0;
			ft_bzero(get_data()->line->str, get_data()->line->capacity);
			get_data()->line->len = 0;
			ft_putstr_fd(MSG_PROMPT, 1);
		}
		get_data()->sigint_received = 1;
		env_set("?", "130");
	}
}

void	sig_quit(int sig)
{
	if (sig == SIGQUIT)
	{
		if (*print_prompt() == 1)
		{
			//printf("\n");
			//ft_putstr_fd(MSG_PROMPT, 1);
			ft_bzero(get_data()->line->str, get_data()->line->capacity);
			get_data()->line->len = 0;
		}
		else
		{
			printf("Quit\n");
			get_data()->sigquit_received = 1;
			env_set("?", "131");
		}
	}
}

void	ctrl_d(t_data *data)
{
	free_data(data);
	env_shutdown();
	write(1, "exit\n", 5);
	exit(0);
}

void	handle_error_code(t_data *data, int status)
{
	char		*status_str;

	status_str = ft_itoa(status >> 8);
	if (data->sigint_received)
	{
		data->sigint_received = 0;
		env_set("?", "130");
	}
	else if (data->sigquit_received)
	{
		data->sigquit_received = 0;
		env_set("?", "131");
	}
	else
		env_set("?", status_str);
	free(status_str);
}
