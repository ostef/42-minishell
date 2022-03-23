/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 14:34:11 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/21 15:34:11 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static void	save_std_fds(t_file fds[3])
{
	fds[STDIN] = dup (STDIN);
	fds[STDOUT] = dup (STDOUT);
	fds[STDERR] = dup (STDERR);
}

static void	restore_std_fds(t_file fds[3])
{
	dup2 (fds[STDIN], STDIN);
	dup2 (fds[STDOUT], STDOUT);
	dup2 (fds[STDERR], STDERR);
	close (fds[STDIN]);
	close (fds[STDOUT]);
	close (fds[STDERR]);
}

t_int	cmd_exec_builtin(t_shell *sh, t_cmd *cmd)
{
	t_file	std_fds[3];
	t_int	exit_status;

	save_std_fds (std_fds);
	if (cmd->next)
		dup2 (cmd->pipe[PIPE_WRITE], STDOUT);
	if (cmd->prev)
		dup2 (cmd->prev->pipe[PIPE_READ], STDIN);
	if (ft_strequ (cmd->args[0], "echo"))
		exit_status = builtin_echo (sh, cmd);
	else if (ft_strequ (cmd->args[0], "cd"))
		exit_status = builtin_cd (sh, cmd);
	else if (ft_strequ (cmd->args[0], "pwd"))
		exit_status = builtin_pwd (sh, cmd);
	else if (ft_strequ (cmd->args[0], "export"))
		exit_status = 1;
	else if (ft_strequ (cmd->args[0], "unset"))
		exit_status = 1;
	else if (ft_strequ (cmd->args[0], "env"))
		exit_status = 1;
	else if (ft_strequ (cmd->args[0], "exit"))
		exit_status = builtin_exit (sh, cmd);
	restore_std_fds (std_fds);
	return (exit_status);
}

t_bool	cmd_is_builtin(t_cmd *cmd)
{
	return (ft_strequ (cmd->args[0], "echo")
		|| ft_strequ (cmd->args[0], "cd")
		|| ft_strequ (cmd->args[0], "pwd")
		|| ft_strequ (cmd->args[0], "export")
		|| ft_strequ (cmd->args[0], "unset")
		|| ft_strequ (cmd->args[0], "env")
		|| ft_strequ (cmd->args[0], "exit"));
}
