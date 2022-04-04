/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 17:17:40 by aandric           #+#    #+#             */
/*   Updated: 2022/04/04 20:15:29 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static t_bool	redir_out(t_redir *redir, t_cmd *cmd)
{
	if (cmd->fd_out)
		close(cmd->fd_out);
	if (redir->kind == RD_OUT_APPEND)
		cmd->fd_out = open(redir->filename,
				O_WRONLY | O_CREAT | O_APPEND, 0666);
	else
		cmd->fd_out = open(redir->filename,
				O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (cmd->fd_out < 0)
	{
		eprint ("%s: %m", redir->filename);
		return (FALSE);
	}
	return (TRUE);
}

static t_bool	redir_in(t_redir *redir, t_cmd *cmd)
{
	if (cmd->fd_in)
		close(cmd->fd_in);
	cmd->fd_in = open(redir->filename, O_RDONLY);
	if (cmd->fd_in < 0)
	{
		eprint ("%s: %m", redir->filename);
		return (FALSE);
	}
	return (TRUE);
}

static t_bool	redir_here(t_shell *sh, t_redir *redir, t_cmd *cmd)
{
	t_file	here_pipe[2];
	t_str	delim;

	if (pipe (here_pipe) == -1)
	{
		eprint ("%m");
		return (FALSE);
	}
	if (cmd->fd_in)
		close (cmd->fd_in);
	cmd->fd_in = here_pipe[PIPE_READ];
	delim = readline("> ");
	while (delim && !ft_strequ(delim, redir->filename))
	{
		ft_fprintln (here_pipe[PIPE_WRITE],
			expand_variables (sh, delim, ft_strlen (delim)));
		free (delim);
		delim = readline("> ");
	}
	free (delim);
	close (here_pipe[PIPE_WRITE]);
	return (TRUE);
}

t_bool	redir_open(t_shell *shell, t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->redir_first;
	while (redir)
	{
		if (redir->kind == RD_OUT || redir->kind == RD_OUT_APPEND)
		{
			if (!redir_out(redir, cmd))
				return (FALSE);
		}
		else if (redir->kind == RD_IN)
		{
			if (!redir_in(redir, cmd))
				return (FALSE);
		}
		else if (redir->kind == RD_IN_HERE)
		{
			if (!redir_here(shell, redir, cmd))
				return (FALSE);
		}
		if (g_globals.exit_exec)
			break ;
		redir = redir->next;
	}
	return (TRUE);
}
