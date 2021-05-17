/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/16 00:24:42 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/17 21:54:24 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			free_history(t_history *history)
{
	int i;

	i = 0;
	if (history->file_history)
	{
		while (history->file_history[i])
		{
			free(history->file_history[i]);
			i++;
		}
		free(history->file_history);
	}
	if (history->path)
		ft_string_destroy(history->path);
}

static void		jump_to_prompt2(t_string *escape, char *itoa, int y)
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
