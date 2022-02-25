/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:38:06 by soumanso          #+#    #+#             */
/*   Updated: 2022/02/25 16:02:54 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_str	cmd_get_argv_buff(t_cmd *cmd)
{
	t_str	buff;
	t_s64	buff_len;
	t_s64	i;
	t_s64	offset;

	buff_len = ft_strlen (cmd->name) + 1;
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		buff_len += ft_strlen (cmd->name) + 1;
		i += 1;
	}
	buff = (t_str)ft_alloc (buff_len, ft_temp ());
	if (!buff)
		return (NULL);
	ft_memcpy (buff, cmd->name, ft_strlen (cmd->name) + 1);
	i = 0;
	offset = ft_strlen (cmd->name) + 1;
	while (cmd->args && cmd->args[i])
	{
		ft_memcpy (buff + offset, cmd->args[i], ft_strlen (cmd->args[i]) + 1);
		offset += ft_strlen (cmd->args[i]) + 1;
		i += 1;
	}
	return (buff);
}

static t_str	*cmd_get_argv(t_cmd *cmd)
{
	t_str	buff;
	t_str	*result;
	t_s64	i;

	buff = cmd_get_argv_buff (cmd);
	if (!buff)
		return (NULL);
	result = (t_str *)ft_alloc (sizeof (t_str) * (cmd->arg_count + 2), ft_temp ());
	if (!result)
		return (NULL);
	i = 0;
	while (i < cmd->arg_count + 1)
	{
		result[i] = buff;
		buff += ft_strlen (buff) + 1;
		i += 1;
	}
	result[i] = NULL;
	return (result);
}

void	cmd_close_all_pipes(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->pipe[PIPE_READ] > STDERR)
			close (cmd->pipe[PIPE_READ]);
		if (cmd->pipe[PIPE_WRITE] > STDERR)
			close (cmd->pipe[PIPE_WRITE]);
		cmd = cmd->prev;
	}
}

void	cmd_exec(t_shell *sh, t_cmd *cmd)
{
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
		if (execve (cmd->name, cmd_get_argv (cmd), sh->env_original) == -1)
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
