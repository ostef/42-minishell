/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:38:06 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/10 17:39:53 by soumanso         ###   ########lyon.fr   */
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

static t_str	*env_list_to_array(t_shell *sh)
{
	t_str	*result;
	t_env	*curr;
	t_int	i;

	result = (t_str *)ft_alloc (sizeof (t_str) * (sh->env_count + 1), ft_temp ());
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
		if (execve (cmd->args[0], cmd->args, env_list_to_array (sh)) == -1)
			exit (128);
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
