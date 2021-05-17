/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 22:42:36 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/17 22:11:53 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "minishell.h"

static int	line_empty(t_string *line)
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

static void	write_char(t_string *line, char *buf, int *flag)
{
	write(1, buf, 1);
	if ((get_data()->x_curr % get_data()->x) == 0 && *buf != '\n')
		write(1, "\n", 1);
	if (*buf != '\n')
	{
		if (!(ft_string_push(line, buf)))
			panic(ERR_MALLOC);
	}
	else
	{
		ft_string_push_char(line, '\0');
		*flag = 1;
	}
}

static int	read_line(t_string *line)
{
	char	buf[11];
	int		ret;
	int		flag;

	flag = 0;
	while (!flag)
	{
		getx(&get_data()->x_curr, &get_data()->y);
		ret = read(0, buf, 10);
		if (ret == -1)
			panic(ERR_READ);
		buf[ret] = '\0';
		if ((*buf > 31 && *buf < 127) || *buf == '\n')
			write_char(line, buf, &flag);
		else if (*buf == 127)
			del_char(line);
		else if (*buf == 4 && line->len == 0)
			ctrl_d(get_data());
		else if (*buf == 27)
			is_arrow(buf);
	}
	return (ret);
}

static void	wait_input(t_data *data)
{
	int		ret;

	if (!(data->line = ft_string_new()))
		panic(ERR_MALLOC);
	ft_putstr_fd(MSG_PROMPT, 1);
	*print_prompt() = 1;
	ret = 1;
	while (ret >= 0)
	{
		get_max_x(data);
		getx(&data->x_prompt, &data->y);
		get_history()->curr_size = 0;
		ret = read_line(data->line);
		if (ret == 0)
			ctrl_d(data);
		if (ret == -1)
			panic(ERR_READ);
		if (!line_empty(data->line))
		{
			*print_prompt() = 0;
			add_history(data);
			return ;
		}
		ft_putstr_fd(MSG_PROMPT, 1);
	}
}

int			main(int argc, char **argv, char **env)
{
	t_data	*data;

	(void)argc;
	(void)argv;
	data = get_data();
	ft_bzero(data, sizeof(t_data));
	env_init(env);
	termcap_init();
	history_init();
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
