/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:38:06 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/23 19:14:01 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static void	cmd_close_prev_pipes(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->pipe[PIPE_READ])
			close (cmd->pipe[PIPE_READ]);
		if (cmd->pipe[PIPE_WRITE])
			close (cmd->pipe[PIPE_WRITE]);
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

	if (cmd->next)
		pipe (cmd->pipe);
	if (cmd_is_builtin (cmd))
		cmd->builtin_exit_status = cmd_exec_builtin (sh, cmd);
	else
	{
		cmd->pid = fork ();
		if (cmd->pid == 0)
		{
			err = cmd_find_path (sh, cmd->args[0], &full_path);
			if (cmd->next || cmd->redir_first)
			{
				if (cmd->redir_first)
					ft_redir(sh, cmd);
				else
					dup2 (cmd->pipe[PIPE_WRITE], STDOUT);
			}
			if (cmd->prev)
			{	
				if (cmd->redir_first)
				{
					dprintf(1, "here");
					ft_redir(sh, cmd);
				}
				else
					dup2 (cmd->prev->pipe[PIPE_READ], STDIN);
			}
			cmd_close_prev_pipes (cmd);
			cmd_handle_error (cmd, err);
			if (execve (full_path, cmd->args, env_list_to_array (sh)) == -1)
				exit (errno);
		}
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
	cmd_close_prev_pipes (line->last);
	cmd = line->first;
	while (cmd)
	{
		if (cmd_is_builtin (cmd))
			status = cmd->builtin_exit_status;
		else
		{
			waitpid (cmd->pid, &status, 0);
			status = WEXITSTATUS (status);
		}
		cmd = cmd->next;
	}
	return (status);
}
