/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/09 22:35:47 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/15 20:42:41 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "minishell.h"

void			add_line(char ***hist, char *line)
{
	int		i;
	char	**tmp;

	if (!(*hist))
	{
		if (!(*hist = malloc(sizeof(char *) * 1)))
			panic(ERR_MALLOC);
		(*hist)[0] = NULL;
	}
	i = 0;
	while ((*hist)[i])
		i++;
	if (!(tmp = malloc(sizeof(char *) * (i + 2))))
		panic(ERR_MALLOC);
	i = -1;
	while ((*hist)[++i])
		tmp[i] = (*hist)[i];
	if (!(tmp[i] = ft_strdup(line)))
		panic(ERR_MALLOC);
	tmp[i + 1] = NULL;
	free(*hist);
	*hist = tmp;
}

void			add_history(t_data *data)
{
	if (!data->line->str)
		return ;
	if (data->history.file_size != 0 &&
		ft_strcmp(data->line->str,
		data->history.file_history[data->history.file_size - 1]) == 0)
		return ;
	add_line(&data->history.file_history, data->line->str);
	data->history.file_size++;
}

static void		read_hist(t_history *history)
{
	char	*line;
	int		gnl;

	gnl = 1;
	while (gnl == 1)
	{
		gnl = get_next_line(history->hist_fd, &line);
		if (gnl == 1)
		{
			add_line(&history->file_history, line);
			history->file_size++;
		}
	}
	history->write_flag = history->file_size;
}

void			open_hist(t_history *history)
{
	history->path = ft_string_new();
	if (!(ft_string_push(history->path, getenv("HOME"))))
		panic(ERR_MALLOC);
	if (!(ft_string_push(history->path, "/.bash_history")))
		panic(ERR_MALLOC);
	history->hist_fd = open(history->path->str, O_RDONLY);
	if (history->hist_fd != -1)
	{
		read_hist(history);
		close(history->hist_fd);
	}
}

void			jump_to_prompt2(t_string *escape, char *itoa, int y)
{
	itoa = ft_itoa(y);
	if (!(ft_string_push(escape, "\x1b[") && ft_string_push(escape, itoa)))
		panic(ERR_MALLOC);
	free(itoa);
	if (!ft_string_push_char(escape, 'F'))
		panic(ERR_MALLOC);
	write(1, escape->str, escape->len);
	ft_string_clear(escape);
	itoa = ft_itoa(get_data()->x_prompt);
	if (!(ft_string_push(escape, "\x1b[") && ft_string_push(escape, itoa)))
		panic(ERR_MALLOC);
	free(itoa);
	if (!ft_string_push_char(escape, 'G'))
		panic(ERR_MALLOC);
	write(1, escape->str, escape->len);
	write(1, "\x1b[0J", 4);
}

void			jump_to_prompt(void)
{
	t_string	*escape;
	char		*itoa;
	int			y;

	get_max_x(get_data());
	y = get_data()->line->len / get_data()->x;
	escape = ft_string_new();
	itoa = NULL;
	if (y == 0)
	{
		itoa = ft_itoa(get_data()->x_prompt);
		if (!(ft_string_push(escape, "\x1b[") && ft_string_push(escape, itoa)))
			panic(ERR_MALLOC);
		free(itoa);
		if (!ft_string_push_char(escape, 'G'))
			panic(ERR_MALLOC);
		write(1, escape->str, escape->len);
		write(1, "\x1b[0J", 4);
	}
	else
		jump_to_prompt2(escape, itoa, y);
	ft_string_destroy(escape);
}

void			show_up_hist2(t_history *history, int index, char **tmp)
{
	if (history->curr_size == 1)
	{
		get_data()->old_line = ft_string_new();
		ft_string_push(get_data()->old_line, get_data()->line->str);
		ft_string_clear(get_data()->line);
		if (!(ft_string_push(get_data()->line, history->file_history[index])))
			panic(ERR_MALLOC);
		write(1, get_data()->line->str, get_data()->line->len);
	}
	else
	{
		if (ft_strcmp(get_data()->line->str, *tmp) != 0)
		{
			free(*tmp);
			*tmp = ft_strdup(get_data()->line->str);
		}
		ft_string_clear(get_data()->line);
		if (!(ft_string_push(get_data()->line, history->file_history[index])))
			panic(ERR_MALLOC);
		write(1, get_data()->line->str, get_data()->line->len);
	}
}

void			show_up_hist(t_history *history)
{
	int		index;
	char	**tmp;

	if (history->file_size > 0 &&
		history->file_size - history->curr_size - 1 >= 0)
	{
		jump_to_prompt();
		history->curr_size++;
		index = history->file_size - history->curr_size;
		tmp = &history->file_history[index + 1];
		show_up_hist2(history, index, tmp);
	}
}

void			show_do_hist2(t_history *history, int index)
{
	if (history->file_history[index])
	{
		ft_string_clear(get_data()->line);
		if (!(ft_string_push(get_data()->line,
				history->file_history[index])))
			panic(ERR_MALLOC);
		write(1, get_data()->line->str, get_data()->line->len);
	}
	else
	{
		ft_string_clear(get_data()->line);
		ft_string_push(get_data()->line, get_data()->old_line->str);
		write(1, get_data()->line->str, get_data()->line->len);
	}
}

void			show_do_hist(t_history *history)
{
	int		index;
	char	**tmp;

	if (history->curr_size > 0)
	{
		jump_to_prompt();
		history->curr_size--;
		index = history->file_size - history->curr_size;
		tmp = &history->file_history[index - 1];
		if (ft_strcmp(get_data()->line->str, *tmp) != 0)
		{
			free(*tmp);
			if (!(*tmp = ft_strdup(get_data()->line->str)))
				panic(ERR_MALLOC);
		}
		show_do_hist2(history, index);
	}
}

t_history		*get_history(void)
{
	static	t_history
}

int				init_history(t_history *history)
{
	open_hist(history);
	return (1);
}
