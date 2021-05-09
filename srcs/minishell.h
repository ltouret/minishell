/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 19:57:52 by ltouret           #+#    #+#             */
/*   Updated: 2021/05/09 14:45:10 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <stdio.h>
# include <sys/stat.h>
# include <limits.h>
# include <string.h>
# include <errno.h>
# include <sys/types.h>
# include "libft.h"

# define MSG_PROMPT "$ "

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

typedef struct		s_cd_opts
{
	char	*home;
	char	*path;
	char	*pwd;
	char	*finalpath;
}					t_cd_opts;

typedef enum		e_env_equality
{
	ENV_EQ_MISFORMAT = -1,
	ENV_EQ_NONE = 0,
	ENV_EQ_UNIFIED = 1
}					t_env_equality;

typedef enum		e_builtin
{
	BI_NONE = -1,
	BI_ECHO,
	BI_CD,
	BI_PWD,
	BI_EXPORT,
	BI_UNSET,
	BI_ENV,
	BI_EXIT,
	BUILTINS_COUNT
}					t_builtin;

typedef struct		s_data
{
	char			**env;
	t_string		*line;
	int				sigint_received;
	int				sigquit_received;
	int				x_prompt; // does this goes here or create new data?
	int				x_curr;
	int				y_curr;
	int				y_coun;
	int				y;
	int				x;
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
	bool			quote;
	bool			dquote;
}					t_pp_state;

t_data				*get_data(void);
int					*print_prompt(void);
void				panic(t_error err_code);
void				print_warning(t_error err_code);
void				handle_error_code(t_data *data, int status);
void				ctrl_d(t_data *data);
void				sig_int(int sig);
void				sig_quit(int sig);
t_error				parse_line(t_data *data);
void				parse_cmds(t_data *data);
void				go_to_redir_end(t_string *pipe_cmd, unsigned int *i);
t_string			*extract_redir(t_string *pipe_cmd, unsigned int i);
int					in_quote(t_pipe_cmd *pipe_cmd, int i, int inside_quote);
t_string			*get_next_redir(t_pipe_cmd *pipe_cmd);
t_error				parse_redirs(t_data *data);
void				post_process(t_data *data);
void				pp_realloc(t_pp_state *st, t_string *str, size_t capacity);
void				pp_replace(t_pp_state *st, ssize_t oldlen,
					char *newword, t_string *str);
void				pp_strip(t_pp_state *st, ssize_t striplen, t_string *str);
void				process_env(t_string *cmd, t_pp_state *st, char *cursor);
void				process_escape(t_string *cmd, t_pp_state *st, char *cursor);
bool				process_space(t_string *cmd, t_pp_state *st, char *cursor);
t_env				**env_dictionary(void);
void				env_init(char **environ);
void				env_shutdown(void);
t_env				*env_get(const char *key);
char				*env_get_value(const char *key);
char				**env_get_array(void);
void				env_clear_array(char **envs);
t_env				*env_create(char *equality);
t_env				*env_set(const char *key, const char *value);
void				env_unset(const char *key);
t_env_equality		env_isvalid_equality(const char *str, bool sanitizename);
char				*env_parse_key(const char *eqlt);
char				*env_parse_value(const char *eqlt);
char				*env_toequality(t_env *env);
size_t				env_listlen(t_env *list);
bool				env_verify_name(const char *str);
bool				env_isinternal(const char *key);
bool				env_isinternal_char(const char c);
size_t				env_get_name_len(const char *str);
void				close_fds(t_data *data, t_cmd *cmd, t_pipe_cmd *pipe_cmd);
void				exec_process(t_data *data);
int					exec_builtin(t_builtin bi, size_t argc, char **argv);
t_error				builtins(t_builtin builtin, size_t argc, char **argv);
t_builtin			get_builtin(char *command);
void				ex_pipe_cmd(t_data *data, t_cmd *cmd, t_pipe_cmd *pipe_cmd);
char				*whereis(const char *filepath);
t_error				builtin_cd(size_t argc, char **argv);
t_error				builtin_echo(size_t argc, char **argv);
t_error				builtin_pwd(size_t argc, char **argv);
t_error				builtin_export(size_t argc, char **argv);
t_error				builtin_unset(size_t argc, char **argv);
t_error				builtin_env(size_t argc, char **argv);
t_error				builtin_exit(size_t argc, char **argv);
t_error				path_pwd(char **str);
t_error				path_chdir(const char *newpath, const char *oldpath);
bool				dir_exists(const char *path, bool follow);
bool				file_exists(const char *path);
char				*path_join(const char *begin, const char *end);
void				free_data(t_data *data);
void				free_cmds(t_list *cmds);
void				free_pipe_cmds(t_list *pipe_cmd_elem);

#endif
