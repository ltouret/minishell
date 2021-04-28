/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 19:58:16 by ltouret           #+#    #+#             */
/*   Updated: 2021/04/28 21:42:35 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "libft.h"
#include "minishell.h"

static void		skip_start(t_string *redir, int *i)
{
	if (redir->str[*i] == '>')
		(*i)++;
	while (redir->str[*i] && ft_strchr("\f\t \n\r\v", redir->str[*i]))
		(*i)++;
}

static t_string	*extract_file(t_string *redir)
{
	t_string	*file;
	int			i;
	int			quote;
	int			dquote;

	dquote = 0;
	quote = 0;
	file = ft_string_new();
	i = 1;
	skip_start(redir, &i);
	while (redir->str[i])
	{
		if (redir->str[i] == '"' && !quote)
			dquote = !dquote;
		else if (redir->str[i] == '\'' && !dquote)
			quote = !quote;
		else if ((redir->str[i] == '<' || redir->str[i] == '>')
			&& !quote && !dquote)
			break ;
		else
			ft_string_push_char(file, redir->str[i]);
		i++;
	}
	return (file);
}

static void		handle_redir_in(t_pipe_cmd *pipe_cmd,
			t_string *redir, t_string *file)
{
	if (!ft_strncmp("<", redir->str, 1))
	{
		if (pipe_cmd->fd_in > 2)
			close(pipe_cmd->fd_in);
		pipe_cmd->fd_in = open(file->str, 0);
		if (pipe_cmd->fd_in == -1)
			print_warning(ERR_OPEN);
	}
}

static void		handle_redir(t_pipe_cmd *pipe_cmd, t_string *redir)
{
	t_string	*file;

	file = extract_file(redir);
	if (!ft_strncmp(">>", redir->str, 2))
	{
		if (pipe_cmd->fd_out > 2)
			close(pipe_cmd->fd_out);
		pipe_cmd->fd_out = open(file->str,
				O_RDWR | O_APPEND | O_CREAT, 00644);
		if (pipe_cmd->fd_out == -1)
			print_warning(ERR_OPEN);
	}
	else if (!ft_strncmp(">", redir->str, 1))
	{
		if (pipe_cmd->fd_out > 2)
			close(pipe_cmd->fd_out);
		pipe_cmd->fd_out = open(file->str,
				O_RDWR | O_TRUNC | O_CREAT, 00644);
		if (pipe_cmd->fd_out == -1)
			print_warning(ERR_OPEN);
	}
	else
		handle_redir_in(pipe_cmd, redir, file);
	ft_string_destroy(file);
}

t_error			parse_redirs(t_data *data)
{
	t_list		*cmd_elem;
	t_list		*pipe_cmd_elem;
	t_string	*next_redir;

	cmd_elem = data->cmds;
	while (cmd_elem)
	{
		pipe_cmd_elem = ((t_cmd *)cmd_elem->content)->pipe_cmds;
		while (pipe_cmd_elem)
		{
			next_redir = get_next_redir(pipe_cmd_elem->content);
			while (next_redir)
			{
				handle_redir(pipe_cmd_elem->content, next_redir);
				ft_string_destroy(next_redir);
				if (((t_pipe_cmd *)pipe_cmd_elem->content)->fd_in == -1
					|| ((t_pipe_cmd *)pipe_cmd_elem->content)->fd_out == -1)
					return (ERR);
				next_redir = get_next_redir(pipe_cmd_elem->content);
			}
			pipe_cmd_elem = pipe_cmd_elem->next;
		}
		cmd_elem = cmd_elem->next;
	}
	return (OK);
}
