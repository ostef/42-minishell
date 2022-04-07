/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:38:06 by soumanso          #+#    #+#             */
/*   Updated: 2022/04/07 15:46:49 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static t_bool	pre_exec(t_shell *sh, t_cmd_line *line)
{
	t_cmd	*cmd;
	t_file	saved_stdin;

	saved_stdin = dup (STDIN);
	g_globals.handled_signal = 0;
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
		{
			cmd->has_errors = TRUE;
			dup2 (saved_stdin, STDIN);
			return (FALSE);
		}
		cmd = cmd->next;
	}
	dup2 (saved_stdin, STDIN);
	close (saved_stdin);
	return (TRUE);
}

static t_int	wait_for_cmds(t_cmd_line *line)
{
	t_int	status;
	t_cmd	*cmd;

	status = EXIT_SUCCESS;
	cmd = line->first;
	while (cmd)
	{
		if (cmd->has_errors)
			status = EXIT_FAILURE;
		else if (cmd_is_builtin (cmd))
			status = cmd->builtin_exit_status;
		else if (cmd->args_count > 0)
		{
			waitpid (cmd->pid, &status, 0);
			status = WEXITSTATUS (status);
		}
		cmd = cmd->next;
	}
	if (g_globals.handled_signal == SIGINT)
		return (g_globals.exit_status);
	return (status);
}

t_int	cmd_line_exec(t_shell *sh, t_cmd_line *line)
{
	t_cmd	*cmd;

	if (!pre_exec (sh, line))
		return (EXIT_FAILURE);
	tcsetattr(0, TCSANOW, &sh->old_termios);
	g_globals.handled_signal = 0;
	signal(SIGINT, exec_signal_handler);
	signal(SIGQUIT, exec_signal_handler);
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
