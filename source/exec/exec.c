/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:38:06 by soumanso          #+#    #+#             */
/*   Updated: 2022/04/01 19:01:23 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static void	cmd_close_files_up_to(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->pipe[PIPE_READ] > STDERR)
			close (cmd->pipe[PIPE_READ]);
		if (cmd->pipe[PIPE_WRITE] > STDERR)
			close (cmd->pipe[PIPE_WRITE]);
		if (cmd->fd_in > STDERR)
			close (cmd->fd_in);
		if (cmd->fd_out > STDERR)
			close (cmd->fd_out);
		cmd = cmd->prev;
	}
}

static t_str	*env_list_to_array(t_shell *sh)
{
	t_str	*result;
	t_env	*curr;
	t_int	i;

	result = (t_str *)ft_alloc (
			sizeof (t_str) * (sh->env_count + 1), ft_temp ());
	if (!result)
		return (NULL);
	i = 0;
	curr = sh->env_first;
	while (curr)
	{
		if (curr->val)
			result[i] = ft_fmt (ft_temp (), "%s=%s", curr->name, curr->val);
		else
			result[i] = curr->name;
		i += 1;
		curr = curr->next;
	}
	result[i] = NULL;
	return (result);
}

static void	cmd_handle_error(t_cmd *cmd, t_err err)
{
	if (err == ERR_CMD_PERM)
	{
		eprint ("%s: Permission denied", cmd->args[0]);
		exit (126);
	}
	else if (err != OK)
	{
		if (err == ERR_CMD_IS_DIR)
			eprint ("%s: is a directory", cmd->args[0]);
		else if (err == ERR_CMD_NO_SUCH_FILE)
			eprint ("%s: No such file or directory", cmd->args[0]);
		else if (err == ERR_CMD_NOT_FOUND)
			eprint ("%s: command not found", cmd->args[0]);
		exit (127);
	}
}

void	cmd_exec(t_shell *sh, t_cmd *cmd)
{
	t_cstr	full_path;
	t_err	err;

	if (cmd_is_builtin (cmd))
		cmd->builtin_exit_status = cmd_exec_builtin (sh, cmd);
	else
	{
		cmd->pid = fork ();
		if (cmd->pid == 0)
		{
			if (cmd->fd_out)
				dup2 (cmd->fd_out, STDOUT);
			else if (cmd->next)
				dup2 (cmd->pipe[PIPE_WRITE], STDOUT);
			if (cmd->fd_in)
				dup2 (cmd->fd_in, STDIN);
			else if (cmd->prev)
				dup2 (cmd->prev->pipe[PIPE_READ], STDIN);
			cmd_close_files_up_to (cmd);
			err = cmd_find_path (sh, cmd->args[0], &full_path);
			cmd_handle_error (cmd, err);
			execve (full_path, cmd->args, env_list_to_array (sh));
			exit (1);
		}
	}
}

t_int	cmd_line_exec(t_shell *sh, t_cmd_line *line)
{
	t_int	status;
	t_cmd	*cmd;

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
		cmd = cmd->next;
	}
	tcsetattr(0, TCSANOW, &sh->old_termios);
	signal(SIGINT, put_nl);
	signal(SIGQUIT, quit_3);
	status = 0;
	cmd = line->first;
	while (cmd)
	{
		if (cmd->args_count > 0 && !cmd->has_errors)
			cmd_exec (sh, cmd);
		cmd = cmd->next;
	}
	cmd_close_files_up_to (line->last);
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
