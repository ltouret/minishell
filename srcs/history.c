/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/09 22:35:47 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/10 00:07:39 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "minishell.h"

static int		add_line(char ***hist, char *line)
{
	int		i;
	char	**tmp;

	if (!(*hist))
	{
		if (!(*hist = malloc(sizeof(char *) * 1)))
			return (0); // err mall
		(*hist)[0] = NULL;
	}
	i = 0;
	while ((*hist)[i])
		i++;
	if (!(tmp = malloc(sizeof(char *) * (i + 2))))
		return (0); // err mall
	i = -1;
	while ((*hist)[++i])
		tmp[i] = (*hist)[i];
	if (!(tmp[i] = ft_strdup(line)))
		return (0);
	tmp[i + 1] = NULL;
	free(*hist);
	*hist = tmp;
	return (1); //check this and return type
}

void	add_history(t_data *data)
{
	if (!data->line->str)
		return ;
	add_line(&data->history.my_history, data->line->str);
	data->history.my_size++;
}

static void	read_hist(t_history *history)
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

void		print_hist(t_history *history, int my_history)
//erase dis, not really needed
{
	int		i;

	if (!my_history)
	{
		i = -1;
		while (history->file_size > ++i)
			printf("%d %s\n", i + 1, history->file_history[i]);
	}
	i = -1;
	while (history->my_size > ++i)
		printf("%d %s\n", history->file_size + i + 1, history->my_history[i]);
}

static int		open_hist(t_history *history)
{
	//TODO add protection if file doesnt exist!!
	// by creating teh damn file
	history->path = ft_string_new();
	if (!(ft_string_push(history->path,  getenv("HOME"))))
		return (0); // err malloc
	if (!(ft_string_push(history->path,  "/.bash_history")))
		return (0); // err malloc
	history->hist_fd = open(history->path->str, O_RDONLY);
	if (history->hist_fd == -1)
		return (1); // err no file found - set errno!!
	read_hist(history);
	close(history->hist_fd);
	return (1); //check this and return type
}

void			show_up_hist(t_history *history)
{
	//printf("curr %d\n", history->curr_size);
		if (history->my_size > 0 &&
			history->my_size > history->curr_size - 1)
		{
			history->curr_size = history->my_size - 1;
			printf("%s\n", history->my_history[history->curr_size]);
			return ;
		}
		else if (history->file_size > 0 &&
			history->file_size > history->curr_size - 1)
		{
			history->curr_size = history->file_size - 1;
			printf("%s\n", history->file_history[history->curr_size]);
			return ;
		}
		//else //no file?
	history->curr_size--;
	printf("curr %d\n", history->curr_size);
}

int				init_history(t_history *history)
{
	//TODO add protection if file doesnt exist!!
	open_hist(history); // add error if cant find
	//for (int i = 0; i < 100; i++)
	//	add_history(history, "im alive!");
	//print_hist(history);
	return (1); //check this and return type
}
