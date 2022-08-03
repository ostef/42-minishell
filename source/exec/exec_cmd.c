/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 20:09:51 by soumanso          #+#    #+#             */
/*   Updated: 2022/08/03 23:33:22 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	cmd_dup_files(t_cmd *cmd)
{
	if (cmd->fd_out)
		dup2 (cmd->fd_out, STDOUT);
	else if (cmd->next)
		dup2 (cmd->pipe[PIPE_WRITE], STDOUT);
	if (cmd->fd_in)
		dup2 (cmd->fd_in, STDIN);
	else if (cmd->prev)
		dup2 (cmd->prev->pipe[PIPE_READ], STDIN);
}

void	cmd_close_files_up_to(t_cmd_line *line, t_cmd *up_to)
{
	t_cmd	*cmd;

	cmd = line->first;
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
		if (cmd == up_to)
			break ;
		cmd = cmd->next;
	}
}

static t_str	*env_list_to_array(t_shell *sh)
{
	t_str	*result;
	t_env	*curr;
	t_int	i;

	result = (t_str *)ft_alloc (
			sizeof (t_str) * (sh->env_count + 1), sh->arena);
	if (!result)
		return (NULL);
	i = 0;
	curr = sh->env_first;
	while (curr)
	{
		if (curr->val)
			result[i] = ft_fmt (sh->arena, "%s=%s", curr->name, curr->val);
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
		exit (EXIT_CMD_NOT_EXECUTABLE);
	}
	else if (err != OK)
	{
		if (err == ERR_CMD_IS_DIR)
			eprint ("%s: is a directory", cmd->args[0]);
		else if (err == ERR_CMD_NO_SUCH_FILE)
			eprint ("%s: No such file or directory", cmd->args[0]);
		else if (err == ERR_CMD_NOT_FOUND)
			eprint ("%s: command not found", cmd->args[0]);
		exit (EXIT_CMD_NOT_FOUND);
	}
}

void	cmd_exec(t_shell *sh, t_cmd_line *line, t_cmd *cmd)
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
			cmd_dup_files(cmd);
			cmd_close_files_up_to (line, line->last);
			err = cmd_find_path (sh, cmd->args[0], &full_path);
			cmd_handle_error (cmd, err);
			execve (full_path, cmd->args, env_list_to_array (sh));
			exit (EXIT_FAILURE);
		}
	}
}
