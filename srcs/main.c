//header

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "minishell.h"
#include "libft.h"
 # include <sys/types.h>

int	is_line_empty(t_string *line)
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

int	read_line(t_string *line)
{
	char	buf[11];
	int		ret;
	int		flag;

	flag = 0;
	while (!flag)
	{
		ret = read(0, buf, 10);
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

void	wait_input(t_data *data)
{
	int		ret;

	data->line = ft_string_new();
	if (!data->line)
		panic(ERR_MALLOC);
	ft_putstr_fd(MSG_PROMPT, 1);
	*should_prompt_be_printed() = 1;
	ret = read_line(data->line);
	while (ret >= 0)
	{
		//if (read_ret == 0)
		//	handle_ctrl_d(data);
		if (ret == -1)
			panic(ERR_READ);
		if (!is_line_empty(data->line))
		{
			*should_prompt_be_printed() = 0;
			return ;
		}
		ft_putstr_fd(MSG_PROMPT, 1);
		ret = read_line(data->line);
	}
}

void	sighandler(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		if (*should_prompt_be_printed())
		{
			ft_bzero(get_data()->line->str, get_data()->line->capacity);
			get_data()->line->len = 0;
			ft_putstr_fd(MSG_PROMPT, 1);
		}
		get_data()->sigint_received = 1;
		/*
		env_set("?", "130");
		*/
	}
	else if (signum == SIGQUIT)
	{
		printf("Quit\n");
		get_data()->sigquit_received = 1;
		/*
		env_set("?", "131");
		*/
	}
}

int		main(int argc, char **argv)
{
	ft_printf("%s\n", "Hey, welcome to my minishell!");
	t_data	*data;
	(void) argc;
	(void) argv;
	data = get_data();
	ft_bzero(data, sizeof(t_data));
	//env here
	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);
	while (1)
	{
		wait_input(data); 
		parse_line(data);
		printf("%s", data->line->str);
		exit(1);
	}
}
