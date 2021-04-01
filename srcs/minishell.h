//header

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <stdio.h>
# include <sys/stat.h>
# include <limits.h>
# include <string.h>
# include <errno.h>
# include "libft.h"

typedef enum		e_error
{
	ERR = -1,
	OK = 0,
	ERR_MALLOC,
	ERR_SYNTAX,
	ERR_READ,
	ERR_UNIMPLENTED,
	ERR_PRINTF,
	ERR_QUOTE,
	ERR_PIPE,
	ERR_EMPTY_COMMAND,
	ERR_TOOMUCH_ARGS,
	ERR_ROOTPARENT,
	ERR_ERRNO,
	ERR_OPEN,
	ERR_UNKNOWN_CMD,
	ERR_UNINIT_ENV_DICO,
	ERR_DOUBLE_INIT_ENV_DICO
}					t_error;

# define MSG_PROMPT "This is a prompt please enter your cmd $ "

typedef struct		s_data
{
	char			**env;
	t_string		*line;
	int				sigint_received;
	int				sigquit_received;
	t_list			*cmds;
}					t_data;

typedef struct		s_cmd
{
	t_string		*cmd;
	t_list			*pipe_cmds;
	bool			should_fork;
}					t_cmd;

typedef struct		s_pipe_cmd
{
	t_string		*pipe_cmd;
	char			*cmd;
	size_t			argc;
	char			**argv;
	bool			forked;
	pid_t			pid;
	int				fd_in;
	int				fd_out;
}					t_pipe_cmd;

typedef struct		s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

t_data		*get_data(void);
int			*should_prompt_be_printed(void);
void		panic(t_error err_code);
t_error		parse_line(t_data *data);
t_string	*get_next_cmd(t_string *line, unsigned int *i);
t_string	*check_err(t_string *next_cmd, int inside_quote, int inside_dquote);
int			other_cmd_after(t_string *line, int i);
void	fill_pipe_fds(t_list *pipe_cmds);
void	skip_until_pipe(t_string *cmd, unsigned int *i);
t_pipe_cmd	*get_next_pipe_cmd(t_string *cmd, unsigned int *i);
t_list	*get_pipes_cmds(t_string *cmd);
void	parse_cmds(t_data *data);

#endif
