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

void		term_echo()
{
	struct termios *s_termios;
	
	s_termios = get_termios();
	s_termios->c_lflag &= ~(ECHO); /* Les touches tapées au clavier ne s'affficheront plus dans le terminal */
	tcsetattr(0, 0, s_termios);
}

int			mychar(int c)
{
	//printf("%d\n", c);
	write(1,&c,1);
	return 1;
}

void		del_char()
{
	char *kr = tgetstr("le", NULL);
	tputs (kr, 1, mychar);
	kr = tgetstr("ce", NULL);
	tputs (kr, 1, mychar);
}

static int	read_line(t_string *line)
{
	char	buf[11];
	int		ret;
	int		flag;

	flag = 0;
	while (!flag)
	{
		ret = read(0, buf, 10);
		if (*buf != 27 || *buf != 127)
		{
			//printf("%d\n", *buf);
			write(1,buf,1);
		}
		if (*buf == 127) // need to erase char in memeory too
		{
			del_char();
			continue;
		}
		if (ret == -1)
			return (-1);
		if (ret == 0 && line->len == 0)
			return (ret);
		buf[ret] = '\0';
		if (!(ft_string_push(line, buf)))
			panic(ERR_MALLOC);
		if (line->str[line->len - 1] == '\n')
			flag = 1;
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

void		term_test(int argc, char **argv)
{
	int ret;
	char *term_type = getenv("TERM");

	ret = tgetent(NULL, term_type);
	(void) ret;

	/* On évite les warnings pour variables non utilisées. */
	char *cl_cap = tgetstr("cl", NULL);
	char *cm_cap = tgetstr("nd", NULL);
	//char *ku = tgetstr("ku", NULL);
	//char *kd = tgetstr("kd", NULL);
	//char *kl = tgetstr("kl", NULL);
	//char *kr = tgetstr("kr", NULL);
	//char *kr = tgetstr("dc", NULL);
	//tputs (kr, 1, mychar);
	//char *kr = tgetstr("ch", NULL);
	//tputs (kr, 1, mychar);
	//wprintw(NULL, kr);
	struct termios *s_termios;
	s_termios = get_termios();
	tcgetattr(0, s_termios);
	s_termios->c_lflag &= ~(ICANON); /* Met le terminal en mode canonique. La fonction Read recevra les entrées clavier en direct sans attendre qu'on appuie sur Enter */
	//s_termios->c_lflag |= (ECHOE); /* Les touches tapées au clavier ne s'affficheront plus dans le terminal */
	//s_termios->c_lflag &= ~(ECHOE); /* Les touches tapées au clavier ne s'affficheront plus dans le terminal */
	tcsetattr(0, 0, s_termios);
	(void)cl_cap;
	(void)cm_cap;
	//tputs (cl_cap, 1, mychar);
	//tputs (cm_cap, 1, mychar);
	//tputs(tgoto(cm_cap, 0, 2), 1, mychar);
	(void)argc;
	(void)argv;
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
	term_test(argc, argv);
	term_echo();
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
		//printf("%s\n",data->line->str);
		free_data(data);
	}
}
