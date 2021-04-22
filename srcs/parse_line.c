//header

#include "libft.h"
#include "minishell.h"

static int	cmd_after(t_string *line, int i)
{
	while (line->str[i] && ft_strchr("\f\t \n\r\v", line->str[i]))
		i++;
	if (line->str[i] == '\0')
		return (0);
	return (1);
}

t_string	*check_err(t_string *next_cmd, int quote, int dquote)
{
	if (dquote || quote)
	{
		print_warning(ERR_QUOTE);
		ft_string_destroy(next_cmd);
		return (NULL);
	}
	if (!next_cmd->str)
	{
		print_warning(ERR_EMPTY_COMMAND);
		ft_string_destroy(next_cmd);
		return (NULL);
	}
	return (next_cmd);
}

static t_string	*next_cmd(t_string *line, unsigned int *i)
{
	t_string	*next_cmd;
	int			quote;
	int			dquote;

	next_cmd = ft_string_new();
	if (!next_cmd)
		panic(ERR_MALLOC);
	dquote = 0;
	quote = 0;
	while (*i < line->len)
	{
		if (line->str[*i] == '"' && !quote && *i > 0
			&& line->str[(*i) - 1] != '\\')
			dquote = !dquote;
		else if (line->str[*i] == '\'' && !dquote && *i > 0
			&& line->str[(*i) - 1] != '\\')
			quote = !quote;
		else if (line->str[*i] == ';' && !quote && !dquote)
		{
			(*i)++;
			break ;
		}
		ft_string_push_char(next_cmd, line->str[*i]);
		(*i)++;
	}
	return (check_err(next_cmd, quote, dquote));
}

t_error	parse_line(t_data *data)
{
	unsigned int	i;
	t_list			*new_elem;
	t_cmd			*cmd;

	data->cmds = NULL;
	i = 0;
	while (i < data->line->len && cmd_after(data->line, i))
	{
		cmd = malloc(sizeof(t_cmd));
		if (!cmd)
			panic(ERR_MALLOC);
		ft_bzero(cmd, sizeof(t_cmd));
		cmd->cmd = next_cmd(data->line, &i);
		if (!cmd->cmd)
		{
			free(cmd);
			return (ERR);
		}
		new_elem = ft_lstnew("", cmd);
		if (!new_elem)
			panic(ERR_MALLOC);
		ft_lstadd_back(&data->cmds, new_elem);
	}
	return (OK);
}
