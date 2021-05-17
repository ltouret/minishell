/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 19:50:52 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/17 22:12:36 by ltouret          ###   ########.fr       */
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
		if (*print_prompt())
		{
			printf("^C\n");
			get_history()->curr_size = 0;
			ft_string_clear(get_data()->line);
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
			get_history()->curr_size = 0;
			ft_string_clear(get_data()->line);
		}
		else
		{
			printf("^\\Quit\n");
			get_data()->sigquit_received = 1;
			env_set("?", "131");
		}
	}
}

void	ctrl_d(t_data *data)
{
	free_data(data);
	env_shutdown();
	termcap_exit();
	free_history(get_history());
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
