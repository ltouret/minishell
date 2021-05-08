/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 22:42:36 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/09 00:03:47 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//header

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

struct termios	*get_termios()
{
	static struct termios s_termios;

	return (&s_termios);
}

int			*get_counter()
{
	static int	i = 0;

	return (&i);
}

int			mychar(int c)
{
	write(1,&c,1);
	return 1;
}

void		print_escape(char *buf)
{
	int		i;
	int		len;

	len = ft_strlen(buf);
	printf("\nlen:%d ", len);
	i = -1;
	while (++i < len)
		printf("%d ", *(buf + i));
	printf("\n");
}

void		del_char(t_string *line)
{
	int		co;
	char	*kr;

	co = tgetnum("co");
	if (line->len > 0 && *get_counter() > ft_strlen(MSG_PROMPT)) 
	{
		if ((*get_counter() % co) == 0)
		{
			kr = tgetstr("up", NULL);
			tputs (kr, 1, mychar);
			kr = tgetstr("ch", NULL);
			tputs(tgoto(kr, 0, co), 1, mychar);
			kr = tgetstr("ce", NULL);
			tputs (kr, 1, mychar);
			ft_string_pop(line);
			(*get_counter())--;
			return;
		}
		kr = tgetstr("le", NULL);
		tputs (kr, 1, mychar);
		kr = tgetstr("dc", NULL);
		tputs (kr, 1, mychar);
		ft_string_pop(line);
		(*get_counter())--;
		//printf("\n%d %d %d %d\n", *kr, *(kr + 1), *(kr + 2), ft_strlen(kr));
		//write(1,kr,3);
	}
	//int li = tgetnum("li");
	//printf("co %d li %d\n", kra, kre);
	//printf("co %d\n", co);
	//tputs(tgoto(kr, 0, 2), 1, mychar);
}

void		new_del_char(t_string *line)
{
	int			col;
	t_string	*buf;
	char		*itoa;

	col = tgetnum("co");
	itoa = ft_itoa(col);
	buf = ft_string_new();
	ft_string_push(buf, "\x1b[");
	ft_string_push(buf, itoa);
	free(itoa);
	if (!(ft_string_push_char(buf, 'G')))
		return ; // malloc err
	if (line->len > 0 && *get_counter() > ft_strlen(MSG_PROMPT)) 
	{
		if ((*get_counter() % col) == 0)
		{
			write(1, "\x1b[F", 3);
			write(1, buf->str, buf->len);
			write(1, "\x1b[X", 3);
			ft_string_pop(line);
			(*get_counter())--;
			return ;
		}
		write(1, "\b", 1);
		write(1, "\x1b[X", 3);
		ft_string_pop(line);
		(*get_counter())--;
	}
}

int			check_newline(t_string *line)
{
	size_t		i;
	char		c;

	i = 0;
	while (i < line->len)
	{
		c = line->str[i];
		if (c != '\n') //maybe check if there is a printable char
			return (1);
		i++;
	}
	return (0);
}

void		is_arrow(char *buf)
{
	//char *kl = tgetstr("kl", NULL);
	//char *kr = tgetstr("kr", NULL);

	//write(1,ku,3);
	//write(1,buf,3);
	if (strncmp(buf, "\x1b[A", 3) == 0)
	{
		//write(1,buf,3);
		printf("up\n");
	}
	if (strncmp(buf, "\x1b[B", 3) == 0)
	{
		//write(1,buf,3);
		printf("down\n");
	}
}

void		getxy()
{
	//This func will get the current col & row (y, x)
	/*
	this func will bge called the first time the code is executed to regulate 
	how the code erases char at the start of a line
	it will be called and give x as starting char so that erase_char func doesnt 
	erase the "start" of my prompt.
	should call this func after each prompt to update starting x of prompt each 
	time. 
	*/
	
	char	buf[10];
	int		i = 0;
	int		ch = 0;
	int		ret = 0;

	write(1, "\x1b[6n", 4);
	(void) buf;
	while (ch != 'R')
	{
		ret = read(0, &ch, 1);
		if (!ret)
			return ;
		buf[i] = ch;
		//printf("buf[%d]: \t%c \t%d\n", i, buf[i], ch);
		i++;
	}
	//printf("%s\n", ft_strchr(buf, '['));
}

static int	read_line(t_string *line)
{
	char	buf[11];
	int		ret;
	int		flag;
	int		co;

	co = tgetnum("co");
	flag = 0;
	while (!flag)
	{
		ret = read(0, buf, 10);
		if ((*buf > 31 && *buf < 127) || *buf == '\n')
		{
			(*get_counter())++;
			write(1,buf,1);
			if ((*get_counter() % co) == 0)
				write(1,"\n",1);
			buf[ret] = '\0';
			if (!(ft_string_push(line, buf)))
				panic(ERR_MALLOC);
			if (line->str[line->len - 1] == '\n')
			{
				*get_counter() = ft_strlen(MSG_PROMPT);
				flag = 1;
			}
		}
		if (*buf == 127)
			new_del_char(line);
		if (*buf == 4 && check_newline(line) == 0)
			ctrl_d(get_data());
		if (*buf == 27)
			is_arrow(buf);
	}
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
	ret = read_line(data->line);
	while (ret >= 0)
	{
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
		ret = read_line(data->line);
	}
}

void		term_test()
// add protections for termios here
{
	char *term_type = getenv("TERM");
	int ret;

	ret = (tgetent(NULL, term_type));
	if (ret == -1 || ret == 0)
		printf("%d\n", ret); // termcap error here!

	struct termios *s_termios;
	s_termios = get_termios();
	tcgetattr(0, s_termios);
	s_termios->c_lflag &= ~(ICANON); /* Met le terminal en mode canonique. La fonction Read recevra les entrées clavier en direct sans attendre qu'on appuie sur Enter */
	s_termios->c_lflag &= ~(ECHO); /* Les touches tapées au clavier ne s'affficheront plus dans le terminal */
	tcsetattr(0, 0, s_termios);
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
	term_test(argc, argv); //merege this shit
	*get_counter() = ft_strlen(MSG_PROMPT);
	while (1)
	{
		wait_input(data);
		parse_line(data);
		parse_cmds(data);
		if (parse_redirs(data) == ERR)
		{
			free_data(data);
			continue ;
		}
		post_process(data);
		exec_process(data);
		printf("%s%s", MSG_PROMPT, data->line->str);
		free_data(data);
	}
}
