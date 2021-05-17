/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/16 21:39:17 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/17 22:13:41 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		get_max_x(t_data *data)
{
	write(1, "\x1b[s", 3);
	write(1, "\x1b[?25l", 6);
	write(1, "\x1b[999C\x1b[999B", 12);
	getx(&data->x, &data->y);
	write(1, "\x1b[u", 3);
	write(1, "\x1b[?25h", 6);
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
		show_up_hist(get_history());
	if (strncmp(buf, "\x1b[B", 3) == 0)
		show_do_hist(get_history());
}
