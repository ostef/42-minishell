/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:38:06 by soumanso          #+#    #+#             */
/*   Updated: 2022/08/03 23:32:28 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static void	set_error(t_cmd_line *line, t_cmd *cmd)
{
	cmd->has_errors = TRUE;
	cmd_close_files_up_to (line, cmd);
}

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
			set_error (line, cmd);
			break ;
		}
		if (!redir_open(sh, cmd))
		{
			set_error (line, cmd);
			break ;
		}
		cmd = cmd->next;
	}
	dup2 (saved_stdin, STDIN);
	close (saved_stdin);
	return (!cmd || !cmd->has_errors);
}

static t_int	handle_signal_exit(t_int status)
{
	if (!WIFSIGNALED (status))
		return (0);
	g_globals.handled_signal = WTERMSIG(status);
	g_globals.exit_status = EXIT_SIGNAL + g_globals.handled_signal;
	if (WTERMSIG(status) == SIGINT)
		ft_print ("\n");
	else if (WTERMSIG(status) == SIGQUIT)
		ft_println ("Quit: 3");
	else if (WTERMSIG (status) == SIGSEGV)
		ft_println ("Segmentation fault");
	else if (WTERMSIG (status) == SIGBUS)
		ft_println ("Bus error");
	else if (WTERMSIG (status) == SIGABRT)
		ft_println ("Aborted");
	else if (WTERMSIG (status) == SIGFPE)
		ft_println ("Floating point exception");
	return (g_globals.exit_status);
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
			if (!cmd->next)
			{
				if (WIFSIGNALED (status))
					status = handle_signal_exit (status);
				else
					status = WEXITSTATUS (status);
			}
		}
		cmd = cmd->next;
	}
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
			cmd_exec (sh, line, cmd);
		cmd = cmd->next;
	}
	cmd_close_files_up_to (line, line->last);
	return (wait_for_cmds (line));
}
