/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/09 22:35:47 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/16 00:28:12 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "minishell.h"

void			add_history(t_data *data)
{
	t_history	*history;

	history = get_history();
	if (!data->line->str)
		return ;
	if (history->file_size != 0 &&
		ft_strcmp(data->line->str,
		history->file_history[history->file_size - 1]) == 0)
		return ;
	add_line(&history->file_history, data->line->str);
	history->file_size++;
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
}

static void		open_hist(t_history *history)
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

t_history		*get_history(void)
{
	static t_history	history;

	return (&history);
}

void			history_init(void)
{
	t_history *history;

	history = get_history();
	open_hist(history);
}
