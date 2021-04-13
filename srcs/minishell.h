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

typedef enum e_env_equality
{
	ENV_EQ_MISFORMAT = -1,
	ENV_EQ_NONE = 0,
	ENV_EQ_UNIFIED = 1
}					t_env_equality;

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

typedef struct		s_pp_state
{
	t_pipe_cmd		*cmd;
	char			*cursor;
	char			*anchor;
	bool			in_quote;
	bool			in_dquote;
}					t_pp_state;

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
void	go_to_redir_end(t_string *pipe_cmd, unsigned int *i);
t_string	*extract_redir(t_string *pipe_cmd, unsigned int i);
int	in_quote(t_pipe_cmd *pipe_cmd, int i, int inside_quote);
t_string	*get_next_redir(t_pipe_cmd *pipe_cmd);
void	skip_start(t_string *redir, int *i);
t_string	*extract_filename(t_string *redir);
void	handle_in(t_pipe_cmd *pipe_cmd, t_string *redir, t_string *filename);
void	handle_redir(t_pipe_cmd *pipe_cmd, t_string *redir);
t_error	parse_redirs(t_data *data);
void	post_process(t_data *data);
void	pp_realloc(t_pp_state *st, t_string *str, size_t capacity);
void	pp_replace(t_pp_state *st, ssize_t oldlen, char *newword, t_string *str);
void	pp_strip(t_pp_state *st, ssize_t striplen, t_string *str);
void	process_env(t_string *cmd, t_pp_state *st, char *cursor);
void	process_escape(t_string *cmd, t_pp_state *st, char *cursor);
bool	process_space(t_string *cmd, t_pp_state *st, char *cursor);
t_env	**env_dictionary(void);
void	env_init_fill(char **environ, t_env **prev_next);
void	env_init(char **environ);
void	env_shutdown(void);
t_env	*env_get(const char *key);
char	*env_get_value(const char *key);
char	**env_get_array(void);
void	env_clear_array(char **envs);
t_env	*env_create(char *equality);
t_env	*env_set(const char *key, const char *value);
void	env_unset(const char *key);
t_env_equality	env_isvalid_equality(const char *str, bool sanitizename);
char	*env_parse_key(const char *eqlt);
char	*env_parse_value(const char *eqlt);
char	*env_toequality(t_env *env);
size_t	env_listlen(t_env *list);
bool	env_verify_name(const char *str);
bool	env_isinternal(const char *key);
bool	env_isinternal_char(const char c);
size_t	env_get_name_len(const char *str);

#endif
