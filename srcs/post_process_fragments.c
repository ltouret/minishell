//header

#include "minishell.h"

void	process_env(t_string *cmd, t_pp_state *st, char *cursor)
{
	size_t	len;
	char	*env_name;
	char	*env_value;

	len = env_get_name_len(cursor + 1);
	//printf("env len %ld\n", len);
	if (!len)
		pp_strip(st, 1, cmd);
	else
	{
		env_name = ft_substr(cursor, 1, len);
		if (!env_name)
			panic(ERR_MALLOC);
		env_value = env_get_value(env_name);
		//printf("env val %s\n", env_value);
		if (!env_value)
			env_value = "";
		free(env_name);
		//printf("process env: \n");
		pp_replace(st, 1 + len, env_value, cmd);
	}
}

void	process_escape(t_string *cmd, t_pp_state *st, char *cursor)
{
	//printf("escape %s\n", cursor);
	if (*(cursor + 1) == '\n')
		pp_strip(st, 2, cmd);
	else if (ft_strchr("$`\"\\", *(cursor + 1)) || !st->in_dquote)
		pp_strip(st, 1, cmd);
	++(st->cursor);
}

bool	process_space(t_string *cmd, t_pp_state *st, char *cursor)
{
	size_t	space_len;

	if (st->in_dquote)
		return (false);
	space_len = 0;
	//printf("space %s\n", cursor);
	while (*cursor && ft_strchr("\f\t  \n\r\v", *(cursor++)))
		++space_len;
	pp_replace(st, space_len, " ", cmd);
	//printf("cmd %s space %s len %ld\n", cmd->str, cursor, space_len);
	return (true);
}
