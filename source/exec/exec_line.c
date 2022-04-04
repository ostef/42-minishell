/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:38:06 by soumanso          #+#    #+#             */
/*   Updated: 2022/04/04 20:11:35 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static t_bool	pre_exec(t_shell *sh, t_cmd_line *line)
{
	t_cmd	*cmd;

	g_globals.exit_exec = FALSE;
	g_globals.saved_stdin = dup (STDIN);
	signal(SIGINT, pre_exec_sigint_handler);
	cmd = line->first;
	while (cmd)
	{
		if (pipe (cmd->pipe) == -1)
		{
			eprint ("%m");
			cmd->has_errors = TRUE;
		}
		if (!redir_open(sh, cmd))
			cmd->has_errors = TRUE;
		if (g_globals.exit_exec)
		{
			dup2 (g_globals.saved_stdin, STDIN);
			return (FALSE);
		}
		cmd = cmd->next;
	}
	dup2 (g_globals.saved_stdin, STDIN);
	close (g_globals.saved_stdin);
	return (TRUE);
}

static t_int	wait_for_cmds(t_cmd_line *line)
{
	t_int	status;
	t_cmd	*cmd;

	status = 0;
	cmd = line->first;
	while (cmd)
	{
		if (cmd->has_errors)
			status = 1;
		else if (cmd_is_builtin (cmd))
			status = cmd->builtin_exit_status;
		else if (cmd->args_count > 0)
		{
			waitpid (cmd->pid, &status, 0);
			status = WEXITSTATUS (status);
		}
		cmd = cmd->next;
	}
	return (status);
}

t_int	cmd_line_exec(t_shell *sh, t_cmd_line *line)
{
	t_int	status;
	t_cmd	*cmd;

	if (!pre_exec (sh, line))
		return (1);
	tcsetattr(0, TCSANOW, &sh->old_termios);
	signal(SIGINT, exec_signal_handler);
	signal(SIGQUIT, exec_signal_handler);
	status = 0;
	cmd = line->first;
	while (cmd)
	{
		if (cmd->args_count > 0 && !cmd->has_errors)
			cmd_exec (sh, cmd);
		cmd = cmd->next;
	}
	cmd_close_files_up_to (line->last);
	return (wait_for_cmds (line));
}