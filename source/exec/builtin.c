/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 14:34:11 by soumanso          #+#    #+#             */
/*   Updated: 2022/04/20 17:14:35 by aandric          ###   ########lyon.fr   */
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

	exit_status = EXIT_FAILURE;
	save_std_fds (std_fds);
	cmd_dup_files(cmd);
	if (ft_strequ (cmd->args[0], "echo"))
		exit_status = builtin_echo (sh, cmd);
	else if (ft_strequ (cmd->args[0], "cd"))
		exit_status = builtin_cd (sh, cmd);
	else if (ft_strequ (cmd->args[0], "pwd"))
		exit_status = builtin_pwd (sh, cmd);
	else if (ft_strequ (cmd->args[0], "export"))
		exit_status = builtin_export_parse(sh, cmd);
	else if (ft_strequ (cmd->args[0], "unset"))
		exit_status = builtin_unset(sh, cmd);
	else if (ft_strequ (cmd->args[0], "env"))
		exit_status = builtin_env(sh);
	else if (ft_strequ (cmd->args[0], "exit"))
		exit_status = builtin_exit (sh, cmd);
	restore_std_fds (std_fds);
	return (exit_status);
}

t_bool	cmd_is_builtin(t_cmd *cmd)
{
	if (cmd->args_count == 0)
		return (FALSE);
	return (ft_strequ (cmd->args[0], "echo")
		|| ft_strequ (cmd->args[0], "cd")
		|| ft_strequ (cmd->args[0], "pwd")
		|| ft_strequ (cmd->args[0], "export")
		|| ft_strequ (cmd->args[0], "unset")
		|| ft_strequ (cmd->args[0], "env")
		|| ft_strequ (cmd->args[0], "exit"));
}
