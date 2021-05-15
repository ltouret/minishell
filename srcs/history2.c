/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/16 00:16:40 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/16 00:24:08 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static void		show_up_hist2(t_history *history, int index, char **tmp)
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

static void		show_do_hist2(t_history *history, int index)
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
