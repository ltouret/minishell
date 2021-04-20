//header

#include "minishell.h"

static bool	process_char(t_string *cmd, t_pp_state *st, char *cursor)
{
	size_t	strip;

	//printf("cmd %s cursor %s\n", cmd->str, cursor);
	strip = 0;
	if (st->in_quote)
	{
		printf("quote");
		if (*cursor == '\\' && *(cursor + 1) == '\'')
		{
			pp_strip(st, 1, cmd);
			++(st->cursor);
		}
		else if (*cursor == '\'' && ++strip)
			st->in_quote = false;
	}
	else if (ft_strchr("\f\t  \n\r\v", *cursor))
		return (process_space(cmd, st, cursor));
	else if (*cursor == '\\')
		process_escape(cmd, st, cursor);
	else if (*cursor == '$')
		{
			process_env(cmd, st, cursor);
			//printf("env cursor %s\n", cmd->str);
		}
	else if (*cursor == '"' && ++strip)
		st->in_dquote = !st->in_dquote;
	else if (*cursor == '\'' && ++strip)
		st->in_quote = true;
	pp_strip(st, strip, cmd);
	return (*(st->cursor + 1) == '\0' && ++st->cursor);
}

static void	push_arg(t_pipe_cmd *cmd, char **old_argv, t_pp_state *st)
{
	//printf("X: ");
	cmd->argv = malloc((cmd->argc + 2) * sizeof(char*));
	if (!cmd->argv)
		panic(ERR_MALLOC);
	if (!cmd->cmd)
	{
		cmd->cmd = ft_substr(st->anchor, 0, st->cursor - st->anchor);
		//printf("cmd %s argc %ld\n", cmd->cmd, cmd->argc);
		*(cmd->argv) = cmd->cmd;
	}
	else
	{
		ft_memcpy(cmd->argv, old_argv, cmd->argc * sizeof(char*));
		*(cmd->argv + cmd->argc) = ft_substr(
				st->anchor, 0, st->cursor - st->anchor);
		free(old_argv);
	}
	//printf("curs %s anch %s cmd %s argc %ld\n", st->cursor, st->anchor, cmd->cmd, cmd->argc);
	if (!*(cmd->argv + cmd->argc))
		panic(ERR_MALLOC);
	*(cmd->argv + ++cmd->argc) = NULL;
	st->anchor = st->cursor + 1;
}

static void	init_state(t_pp_state *st, t_list *pipe_cmds)
{
	ft_bzero(st, sizeof(t_pp_state));
	st->cmd = (t_pipe_cmd *)(pipe_cmds->content);
	st->cursor = st->cmd->pipe_cmd->str;
	//printf("str %s\n", st->cmd->pipe_cmd->str);
	while (*(st->cursor) && ft_strchr("\f\t  \n\r\v", *(st->cursor)))
	{
		//printf("cursor %c %d\n", *st->cursor, *st->cursor);
		st->cursor++;
	}
	st->anchor = st->cursor;
}

void	post_process(t_data *data)
{
	t_list		*cmds;
	t_list		*pipe_cmds;
	t_pp_state	st;

	cmds = data->cmds;
	while (cmds)
	{
		pipe_cmds = (t_list *)(((t_cmd *)(cmds->content))->pipe_cmds);
		while (pipe_cmds)
		{
			init_state(&st, pipe_cmds);
			//printf("pipe cmd: %s cursor: %s anchor %s\n", ((t_pipe_cmd *)pipe_cmds->content)->pipe_cmd->str, st.cursor, st.anchor);
			while (*st.cursor)
			{
				if (process_char(st.cmd->pipe_cmd, &st, st.cursor))
					push_arg(st.cmd, st.cmd->argv, &st);
				if (*st.cursor)
					st.cursor++;
			}
			pipe_cmds = pipe_cmds->next;
		}
		cmds = cmds->next;
	}
}
