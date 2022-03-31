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

t_bool	redir_out(t_shell *shell, t_cmd *cmd)
{
	t_redir	*temp;

	temp = cmd->redir_link;
	while (temp)
	{
		if (cmd->fd_out)
			close(cmd->fd_out);
		cmd->fd_out = open(temp->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (cmd->fd_out < 0)
		{
			eprint ("%s: %m", temp->filename);
			return (FALSE);
		}
		temp = temp->next;
	}
	return (TRUE);
}

t_bool	redir_out_append(t_shell *shell, t_cmd *cmd)
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
			eprint ("%s: %m", temp->filename);
			return (FALSE);
		}
		temp = temp->next;
	}
	return (TRUE);
}

t_bool	redir_in(t_shell *shell, t_cmd *cmd)
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
				eprint ("%s: %m", temp->filename);
				return (FALSE);
			}
		}
		temp = temp->next;
	}
	return (TRUE);
}

t_bool	redir_here(t_shell *shell, t_cmd *cmd)
{
	t_file	here_pipe[2];
	t_redir	*temp;
	t_str	delim;

	if (pipe (here_pipe) == -1)
	{
		eprint ("%m");
		return (FALSE);
	}
	if (cmd->fd_in)
		close (cmd->fd_in);
	cmd->fd_in = here_pipe[PIPE_READ];
	delim = "";
	while (!ft_strequ(delim, cmd->redir_link->filename))
	{
		delim = readline("heredoc> ");
		if (!ft_strequ(delim, cmd->redir_link->filename))
			ft_fprintln (here_pipe[PIPE_WRITE], delim);
	}
	close (here_pipe[PIPE_WRITE]);
	return (TRUE);
}

// penser au cas 
//<< EOF
// heredoc>$PATH
// heredoc>$'PATH'
// Faire une condition specifique pour ce cas

t_bool	redir_open(t_shell *shell, t_cmd *cmd)
{
	t_bool	res;

	res = TRUE;
	cmd->redir_link = cmd->redir_first;
	while (cmd->redir_link)
	{
		if (cmd->redir_link->kind == RD_OUT)
			res &= redir_out(shell, cmd);
		if (cmd->redir_link->kind == RD_IN)
			res &= redir_in(shell, cmd);
		if (cmd->redir_link->kind == RD_OUT_APPEND)
			res &= redir_out_append(shell, cmd);
		if (cmd->redir_link->kind == RD_IN_HERE)
			res &= redir_here(shell, cmd);
		cmd->redir_link = cmd->redir_link->next;
	}
	return (res);
}
