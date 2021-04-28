/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 19:50:35 by ltouret           #+#    #+#             */
/*   Updated: 2021/04/28 19:50:43 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "minishell.h"
#include "libft.h"
#include <sys/types.h>

int	line_empty(t_string *line)
{
	char	**splitted;
	int		ret;

	splitted = ft_split(line->str, "\f\t \n\r\v");
	if (!splitted)
		panic(ERR_MALLOC);
	if (!*splitted)
		ret = 1;
	else
		ret = 0;
	ft_clear_splitted(splitted);
	return (ret);
}

int	read_line(t_string *line)
{
	char	buf[11];
	int		ret;
	int		flag;

	flag = 0;
	while (!flag)
	{
		ret = read(0, buf, 10);
		if (ret == -1)
			return (-1);
		if (ret == 0 && line->len == 0)
			return (ret);
		buf[ret] = '\0';
		if (!(ft_string_push(line, buf)))
			panic(ERR_MALLOC);
		if (line->str[line->len - 1] == '\n')
			flag = 1;
	}
	return (ret);
}

void	wait_input(t_data *data)
{
	int		ret;

	data->line = ft_string_new();
	if (!data->line)
		panic(ERR_MALLOC);
	ft_putstr_fd(MSG_PROMPT, 1);
	*print_prompt() = 1;
	ret = read_line(data->line);
	while (ret >= 0)
	{
		if (ret == 0)
			ctrl_d(data);
		if (ret == -1)
			panic(ERR_READ);
		if (!line_empty(data->line))
		{
			*print_prompt() = 0;
			return ;
		}
		ft_putstr_fd(MSG_PROMPT, 1);
		ret = read_line(data->line);
	}
}

int		main(int argc, char **argv, char **env)
{
	t_data	*data;

	(void) argc;
	(void) argv;
	data = get_data();
	ft_bzero(data, sizeof(t_data));
	env_init(env);
	signal(SIGINT, &sig_int);
	signal(SIGQUIT, &sig_quit);
	while (1)
	{
		wait_input(data);
		parse_line(data);
		parse_cmds(data);
		if (parse_redirs(data) == ERR)
		{
			free_data(data);
			continue ;
		}
		post_process(data); 
		exec_process(data); 
		free_data(data);
	}
}
