/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 17:18:56 by soumanso          #+#    #+#             */
/*   Updated: 2022/02/12 16:18:46 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(t_cmd *cmd)
{
	t_int	i;

	ft_free (cmd->name_or_path, ALLOC_HEAP);
	i = 0;
	while (i < cmd->arg_count)
	{
		ft_free (cmd->args[i], ALLOC_HEAP);
		i += 1;
	}
	ft_free (cmd->args, ALLOC_HEAP);
	ft_memset (cmd, 0, sizeof (t_cmd));
}

void	free_cmd_line(t_cmd_line *line)
{
	t_int	i;

	free (line->str);
	i = 0;
	while (i < line->cmd_count)
	{
		free_cmd (&line->cmds[i]);
		i += 1;
	}
	ft_free (line->cmds, ALLOC_HEAP);
	ft_memset (line, 0, sizeof (t_cmd_line));
}

static t_str *get_argv(t_cmd *cmd)
{
	t_str	*argv;
	t_int	argc;

	argc = (t_int)cmd->arg_count + 1;
	argv = (t_str *)ft_alloc (sizeof (t_str) * (argc + 1), ALLOC_HEAP);
	argv[0] = cmd->name_or_path;
	ft_memcpy (argv + 1, cmd->args, sizeof (t_str) * cmd->arg_count);
	argv[argc] = NULL;
	return (argv);
}

static void	execute_cmd(t_cmd *cmd)
{
	t_str	*args;
	pid_t	pid;
	t_int	status;

	pid = fork ();
	if (pid == 0)
	{
		if (execve (cmd->name_or_path, get_argv (cmd), NULL) == -1)
		{
			ft_fprintln (STDERR, "%m: %s", args[0]);
			exit (1);
		}
	}
	else
	{
		waitpid (pid, &status, 0);
	}
}

void	execute_cmd_line(t_cmd_line *line)
{
	t_int	i;
	t_int	res;
	pid_t	my_pid;

	i = 0;
	while (i < line->cmd_count)
	{
		execute_cmd (&line->cmds[i]);
		i += 1;
	}
}
