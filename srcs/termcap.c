/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/16 21:34:02 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/16 21:35:09 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <term.h>
#include <curses.h>

void		termcap_init(void)
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

void		termcap_exit(void)
{
	struct termios	s_termios;

	if (tcgetattr(0, &s_termios) == -1)
		panic(ERR_UNKNOWN_TERM);
	s_termios.c_lflag |= (ICANON);
	s_termios.c_lflag |= (ECHO);
	if (tcsetattr(0, 0, &s_termios) == -1)
		panic(ERR_UNKNOWN_TERM);
}
