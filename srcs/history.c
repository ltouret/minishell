/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/09 22:35:47 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/11 02:11:30 by ltouret          ###   ########.fr       */
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
	add_line(&data->history.file_history, data->line->str);
	data->history.file_size++;
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
	history->write_flag = history->file_size;
}

void		print_hist(t_history *history)
//erase dis, not really needed
{
	int		i;

	i = -1;
	while (history->file_size > ++i)
		printf("%d %s\n", i + 1, history->file_history[i]);
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
	if (history->hist_fd == -1) //if deosnt exist works anyway? why
		return (1); // err no file found - set errno!!
	read_hist(history);
	close(history->hist_fd);
	return (1); //check this and return type
}

void			show_up_hist(t_history *history)
{
	int	index;
	int	y;

	//TODO create old_line, the first time we get into this func 
	// do old_line = line, line = strdup history[i]
	// if pressed again (up/do) free(history[i]) and history[i] = line
	// then do this each time, free(line) & line = strdup of history[i]
	// when down called until curr_size == 0
	// do free(line), line = old_line
	// code ft_string_dup.c
	// ctrl c will need to free(old_line)
	// code ft_string_clear 
	// example line 26, 27 of ctrl c

	if (history->file_size > 0 &&
		history->file_size - history->curr_size - 1 >= 0)
	{
		history->curr_size++;
		index = history->file_size - history->curr_size;
		//printf("%s\n", history->file_history[index]);
	}
	//ESC[{line};{column}H
	//ESC[#F
	index = get_data()->line->len;
	get_max_x(get_data());
	y = index / get_data()->x;
	if (y == 0)
	{
		write(1, "\x1b[3G", 4); // jump to prompt use with prompt
		write(1, "\x1b[0J", 4); //clean rest of term
	}
	else //check the case that x or y are negative!!!
	{
		write(1, "\x1b[2F", 4); //go up 2 lines use with y
		write(1, "\x1b[3G", 4); // jump to prompt use with prompt
		write(1, "\x1b[0J", 4); //clean rest of term
	}
	//printf("%d %d %d\n", index, y, x);
}

void			show_do_hist(t_history *history)
{
	int index;

	if (history->curr_size > 0)
	{
		history->curr_size--;
		index = history->file_size - history->curr_size;
		if (history->file_history[index]) 
			printf("%s\n", history->file_history[index]);
		else
			printf("here i erase u bish\n"); //this will jus re print line
	}
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
