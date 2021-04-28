/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 19:58:05 by ltouret           #+#    #+#             */
/*   Updated: 2021/04/28 19:58:07 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include "libft.h"
#include "minishell.h"

static void	fill_pipe(t_list *pipe_cmds)
{
	t_list	*current;
	int		pipefd[2];
	int		lastpipefd[2];

	current = pipe_cmds;
	while (current)
	{
		if (current == pipe_cmds && current->next)
		{
			pipe(pipefd);
			((t_pipe_cmd *)current->content)->fd_in = STDIN_FILENO;
			((t_pipe_cmd *)current->content)->fd_out = pipefd[1];
		}
		else if (current != pipe_cmds && current->next)
		{
			lastpipefd[0] = pipefd[0];
			pipe(pipefd);
			((t_pipe_cmd *)current->content)->fd_in = lastpipefd[0];
			((t_pipe_cmd *)current->content)->fd_out = pipefd[1];
		}
		else if (current != pipe_cmds && !current->next)
			((t_pipe_cmd *)current->content)->fd_in = pipefd[0];
		current = current->next;
	}
}

static void	skip_until_pipe(t_string *cmd, unsigned int *i)
{
	int				quote;
	int				dquote;

	quote = 0;
	dquote = 0;
	while (*i <= cmd->len)
	{
		if (cmd->str[*i] == '"' && !quote && *i > 0
			&& cmd->str[*i - 1] != '\\')
			dquote = !dquote;
		else if (cmd->str[*i] == '\'' && !dquote && *i > 0
			&& cmd->str[*i - 1] != '\\')
			quote = !quote;
		else if (cmd->str[*i] == '|' && !quote && !dquote)
		{
			(*i)++;
			break ;
		}
		(*i)++;
	}
}

static t_pipe_cmd	*get_next_pipe_cmd(t_string *cmd, unsigned int *i)
{
	int				start;
	t_pipe_cmd		*next_pipe_cmd;

	start = *i;
	skip_until_pipe(cmd, i);
	next_pipe_cmd = malloc(sizeof(t_pipe_cmd));
	if (!next_pipe_cmd)
		panic(ERR_MALLOC);
	ft_bzero(next_pipe_cmd, sizeof(t_pipe_cmd));
	next_pipe_cmd->pipe_cmd = ft_string_sub(cmd, start, *i - start - 1);
	if (!next_pipe_cmd->pipe_cmd)
		panic(ERR_MALLOC);
	return (next_pipe_cmd);
}

static t_list	*get_pipes_cmds(t_string *cmd)
{
	t_list			*pipes_cmds;
	t_list			*new;
	unsigned int	i;

	pipes_cmds = NULL;
	i = 0;
	while (i < cmd->len)
	{
		new = ft_lstnew("", get_next_pipe_cmd(cmd, &i));
		if (!(new))
			panic(ERR_MALLOC);
		((t_pipe_cmd *)new->content)->fd_in = STDIN_FILENO;
		((t_pipe_cmd *)new->content)->fd_out = STDOUT_FILENO;
		ft_lstadd_back(&pipes_cmds, new);
	}
	return (pipes_cmds);
}

void	parse_cmds(t_data *data)
{
	t_list		*cmd_elem;
	t_cmd		*cmd;

	cmd_elem = data->cmds;
	while (cmd_elem)
	{
		cmd = cmd_elem->content;
		cmd->pipe_cmds = get_pipes_cmds(cmd->cmd);
		fill_pipe(cmd->pipe_cmds);
		cmd_elem = cmd_elem->next;
	}
}
