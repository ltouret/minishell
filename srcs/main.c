/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 22:42:36 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/15 20:39:42 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//check includes!!!

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "minishell.h"
#include "libft.h"
#include <term.h>
#include <curses.h>
#include <sys/types.h>

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

void		getx(int *x, int *y)
{
	char	buf[1000];
	int		i;
	int		ch;

	i = 0;
	ch = 0;
	write(1, "\x1b[6n", 4);
	while (ch != 'R')
	{
		if (!read(0, &ch, 1))
			panic(ERR_UNKNOWN_X);
		buf[i] = ch;
		i++;
	}
	if (x)
		*x = ft_atoi(ft_strchr(buf, ';') + 1);
	if (y)
		*y = ft_atoi(ft_strchr(buf, '[') + 1);
}

void		del_char(t_string *line)
{
	t_string	*buf;
	char		*itoa;

	itoa = ft_itoa(get_data()->x);
	buf = ft_string_new();
	ft_string_push(buf, "\x1b[");
	ft_string_push(buf, itoa);
	free(itoa);
	if (!(ft_string_push_char(buf, 'G')))
		panic(ERR_MALLOC);
	if (line->len > 0)
	{
		if (get_data()->x_curr == 1)
		{
			write(1, "\x1b[F", 3);
			write(1, buf->str, buf->len);
			write(1, "\x1b[X", 3);
			ft_string_pop(line);
			return ;
		}
		write(1, "\b\x1b[X", 4);
		ft_string_pop(line);
	}
	ft_string_destroy(buf);
}

void		is_arrow(char *buf)
{
	if (strncmp(buf, "\x1b[A", 3) == 0)
	{
		show_up_hist(&get_data()->history);
		//printf("up\n");
	}
	if (strncmp(buf, "\x1b[B", 3) == 0)
	{
		//printf("down\n");
		show_do_hist(&get_data()->history);
	}
}

void		get_max_x(t_data *data)
{
	write(1, "\x1b[s", 3);
	write(1, "\x1b[?25l", 6);
	write(1, "\x1b[999C\x1b[999B", 12);
	getx(&data->x, &data->y);
	write(1, "\x1b[u", 3);
	write(1, "\x1b[?25h", 6);
}

static int	read_line(t_string *line)
{
	char	buf[11];
	int		ret;
	int		flag;

	flag = 0;
	//printf("line: %s - %ld\n", line->str, line->len);
	while (!flag)
	{
		getx(&get_data()->x_curr, &get_data()->y);
		ret = read(0, buf, 10);
		if (ret == -1)
			panic(ERR_READ);
		buf[ret] = '\0';
		if ((*buf > 31 && *buf < 127) || *buf == '\n')
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
				flag = 1;
			}
		}
		else if (*buf == 127)
		{
			del_char(line);
		}
		else if (*buf == 4 && line->len == 0)
		{
			ctrl_d(get_data());
		}
		else if (*buf == 27)
			is_arrow(buf);
	}
	//printf("line: %s - %ld\n", line->str, line->len);
	return (ret);
}

static void	wait_input(t_data *data)
{
	int		ret;

	data->line = ft_string_new();
	if (!data->line)
		panic(ERR_MALLOC);
	ft_putstr_fd(MSG_PROMPT, 1);
	*print_prompt() = 1;
	ret = 1;
	while (ret >= 0)
	{
		get_max_x(data);
		getx(&data->x_prompt, &data->y);
		data->history.curr_size = 0;
		ret = read_line(data->line);
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
	}
}

void		init_termcap(void)
{
	char			*term_type;
	struct termios	s_termios;
	int				ret;

	term_type = getenv("TERM");
	if (!term_type)
		panic(ERR_MALLOC);
	ret = (tgetent(NULL, term_type));
	if (ret == -1 || ret == 0)
		panic(ERR_UNKNOWN_TERM);
	if (tcgetattr(0, &s_termios) == -1)
		panic(ERR_UNKNOWN_TERM);
	s_termios.c_lflag &= ~(ICANON);
	s_termios.c_lflag &= ~(ECHO);
	if (tcsetattr(0, 0, &s_termios) == -1)
		panic(ERR_UNKNOWN_TERM);
}

int			main(int argc, char **argv, char **env)
{
	t_data	*data;

	(void)argc;
	(void)argv;
	data = get_data();
	ft_bzero(data, sizeof(t_data));
	env_init(env);
	signal(SIGINT, &sig_int);
	signal(SIGQUIT, &sig_quit);
	init_termcap();
	init_history();
	while (1)
	{
		wait_input(data);
		add_history(data);
		parse_line(data);
		parse_cmds(data);
		if (parse_redirs(data) == ERR)
		{
			free_data(data);
			continue ;
		}
		post_process(data);
		exec_process(data);
		printf("%s%s %ld %ld\n", MSG_PROMPT, data->line->str, data->line->len, data->line->capacity);
		//free_data(data); // deal with history being freed each fucking time
	}
}
