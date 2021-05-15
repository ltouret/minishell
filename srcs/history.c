/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/09 22:35:47 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/15 02:27:11 by ltouret          ###   ########.fr       */
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
	//printf("cmp %d\n", ft_strcmp(data->line->str, data->history.file_history[data->history.file_size - 1]));
	if (data->history.file_size != 0 && ft_strcmp(data->line->str, data->history.file_history[data->history.file_size - 1]) == 0)
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
	//TODO add malloc prot / panic
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

void			jump_to_prompt()
{
	//TODO add malloc prot / panic
	//TODO create old_line, the first time we get into this func 
	// do old_line = line, line = strdup history[i]
	// if pressed again up/down & line modified free(history[i]) and history[i] = line
	// then do this each time, free(line) & line = strdup of history[i]
	// when down called until curr_size == 0
	// do free(line), line = old_line
	// ctrl c will need to free(old_line)
	// example line 26, 27 of ctrl c

	char		*itoa;
	t_string	*escape;
	int			index;
	int			y;

	index = get_data()->line->len;
	get_max_x(get_data());
	y = index / get_data()->x;
	escape = ft_string_new();
	//TODO add panic for malloc
	// && FREE!
	if (y == 0)
	{
		itoa = ft_itoa(get_data()->x_prompt);
		ft_string_push(escape, "\x1b[");
		ft_string_push(escape, itoa);
		if (!ft_string_push_char(escape, 'G')) // panic malloc
			panic(ERR_MALLOC);// panic malloc
		write(1, escape->str, escape->len); // jump to prompt use with prompt
		write(1, "\x1b[0J", 4); //clean rest of term
		free(itoa);
	}
	else //check the case that x or y are negative!!!
	{
		itoa = ft_itoa(y);
		ft_string_push(escape, "\x1b[");
		ft_string_push(escape, itoa);
		if (!ft_string_push_char(escape, 'F'))
			panic(ERR_MALLOC);// panic malloc
		free(itoa);
		write(1, escape->str, escape->len); // jump to prompt use with prompt
		ft_string_clear(escape);
		itoa = ft_itoa(get_data()->x_prompt);
		ft_string_push(escape, "\x1b[");
		ft_string_push(escape, itoa);
		if (!ft_string_push_char(escape, 'G')) // panic malloc
			panic(ERR_MALLOC);// panic malloc
		free(itoa);
		write(1, escape->str, escape->len); // jump to prompt use with prompt
		write(1, "\x1b[0J", 4); //clean rest of term
	}
	ft_string_destroy(escape);
}

void			show_up_hist(t_history *history)
{
	//TODO add malloc prot / panic
	int		index;
	char	**tmp;

	if (history->file_size > 0 &&
		history->file_size - history->curr_size - 1 >= 0)
	{
		jump_to_prompt();
		history->curr_size++;
		index = history->file_size - history->curr_size;
		tmp = &history->file_history[index + 1];
		if (history->curr_size == 1)
		{
			get_data()->old_line = ft_string_new();
			ft_string_push(get_data()->old_line, get_data()->line->str);
			ft_string_clear(get_data()->line);
			ft_string_push(get_data()->line, history->file_history[index]);
			//printf("in %d %s %s\n", history->curr_size, get_data()->old_line->str, get_data()->line->str); 
			write(1, get_data()->line->str, get_data()->line->len); 
		}
		else
		{
			//TODO each time you go up/do free(history[i]) and replace it for line, so if line is modified the history will be modified too
			// maybe add an array with the modified history (i mean the * after the number
			if (ft_strcmp(get_data()->line->str, *tmp) 
				!= 0)
			{
				//printf("tmp %s\n", history->file_history[index]);
				free(*tmp);
				*tmp = ft_strdup(get_data()->line->str);
				//printf("tmp %s\n", *tmp);
			}
			ft_string_clear(get_data()->line);
			ft_string_push(get_data()->line, history->file_history[index]);
			write(1, get_data()->line->str, get_data()->line->len); 
		}
	}
	//printf("%d %d %d\n", index, y, x);
}

void			show_do_hist(t_history *history)
{
	//TODO add malloc prot / panic
	int		index;
	char	**tmp;

	if (history->curr_size > 0)
	{
		jump_to_prompt();
		history->curr_size--;
		index = history->file_size - history->curr_size;
		tmp = &history->file_history[index - 1];
		if (ft_strcmp(get_data()->line->str, *tmp) 
			!= 0)
		{
			//printf("tmp %s\n", history->file_history[index]);
			free(*tmp);
			*tmp = ft_strdup(get_data()->line->str);
			//printf("tmp %s\n", *tmp);
		}
		if (history->file_history[index]) 
		{
			ft_string_clear(get_data()->line);
			ft_string_push(get_data()->line, history->file_history[index]);
			write(1, get_data()->line->str, get_data()->line->len); 
			//printf("%s\n", history->file_history[index]);
		}
		else
		{
			ft_string_clear(get_data()->line);
			ft_string_push(get_data()->line, get_data()->old_line->str); 
			write(1, get_data()->line->str, get_data()->line->len); 
			//printf("here i erase u bish\n"); //this will jus re print line
		}
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
