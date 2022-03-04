/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:38:06 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/04 17:33:37 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cmd_close_all_pipes(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->next)
		{
			close (cmd->pipe[PIPE_READ]);
			close (cmd->pipe[PIPE_WRITE]);
		}
		cmd = cmd->prev;
	}
}

void	cmd_exec(t_shell *sh, t_cmd *cmd)
{
	ft_println ("Executing '%s'.", cmd->args[0]);
	if (cmd->next)
		pipe (cmd->pipe);
	cmd->pid = fork ();
	if (cmd->pid == 0)
	{
		if (cmd->next)
			dup2 (cmd->pipe[PIPE_WRITE], STDOUT);
		if (cmd->prev)
			dup2 (cmd->prev->pipe[PIPE_READ], STDIN);
		cmd_close_all_pipes (cmd);
		if (execve (cmd->args[0], cmd->args, sh->env_original) == -1)
			exit (1);
	}
}

t_int	cmd_line_exec(t_shell *sh, t_cmd_line *line)
{
	t_int	status;
	t_cmd	*cmd;

	status = 0;
	cmd = line->first;
	while (cmd)
	{
		cmd_exec (sh, cmd);
		cmd = cmd->next;
	}
	cmd_close_all_pipes (line->last);
	cmd = line->first;
	while (cmd)
	{
		waitpid (cmd->pid, &status, 0);
		cmd = cmd->next;
	}
	return (status);
}
