/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 17:17:40 by aandric           #+#    #+#             */
/*   Updated: 2022/03/25 20:51:08 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

t_bool	ft_redir_out(t_shell *shell, t_cmd *cmd)
{
	int		fd_out;
	t_redir	*temp;

	temp = cmd->redir_link;
	while (temp)
	{
		if (cmd->fd_out)
			close(cmd->fd_out);
		cmd->fd_out = open(temp->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd_out < 0)
		{
			write(2, "outfile not found\n", 18);
			return (0);
		}
		temp = temp->next;
	}
	return (1);
}

t_bool	ft_redir_out_append(t_shell *shell, t_cmd *cmd)
{
	int		fd_out;
	t_redir	*temp;

	temp = cmd->redir_link;
	while (temp)
	{
		if (cmd->fd_out)
			close(cmd->fd_out);
		cmd->fd_out = open(temp->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (fd_out < 0)
		{
			write(2, "outfile not found\n", 18);
			return (0);
		}
		temp = temp->next;
	}
	return (1);
}

t_bool	ft_redir_in(t_shell *shell, t_cmd *cmd)
{
	t_redir	*temp;

	temp = cmd->redir_link;
	while (temp)
	{
		if (temp->kind == RD_IN)
		{
			if (cmd->fd_in)
				close(cmd->fd_in);
			cmd->fd_in = open(temp->filename, O_RDONLY);
			if (cmd->fd_in < 0)
			{
				write(2, "infile not found\n", 17);
				return (0);
			}
		}
		temp = temp->next;
	}
	return (1);
}

t_bool	ft_redir_here(t_shell *shell, t_cmd *cmd)
{
	t_redir	*temp;
	t_str	here_line;
	t_str	delim;

	here_line = "";
	delim = readline("heredoc>");
	here_line = ft_fmt(ft_temp(), "%s\n", delim);
	while (!ft_strequ(delim, cmd->redir_link->filename))
	{
		delim = readline("heredoc>");
		if (!ft_strequ(delim, cmd->redir_link->filename))
			here_line = ft_fmt(ft_temp(), "%s%s\n", here_line, delim);
	}
	write(cmd->pipe[PIPE_WRITE], here_line, ft_strlen(here_line));
	cmd->fd_in = cmd->pipe[PIPE_READ];
	return (1);
}

// penser au cas 
//<< EOF
// heredoc>$PATH
// heredoc>$'PATH'
// Faire une condition specifique pour ce cas

t_bool	ft_redir(t_shell *shell, t_cmd *cmd)
{
	cmd->redir_link = cmd->redir_first;
	while (cmd->redir_link)
	{
		if (cmd->redir_link->kind == RD_OUT)
			ft_redir_out(shell, cmd);
		if (cmd->redir_link->kind == RD_IN)
			ft_redir_in(shell, cmd);
		if (cmd->redir_link->kind == RD_OUT_APPEND)
			ft_redir_out_append(shell, cmd);
		if (cmd->redir_link->kind == RD_IN_HERE)
			ft_redir_here(shell, cmd);
		cmd->redir_link = cmd->redir_link->next;
	}
	return (0);
}
