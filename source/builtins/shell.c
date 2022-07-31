/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 15:33:04 by soumanso          #+#    #+#             */
/*   Updated: 2022/07/31 12:25:10 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	should_ignore_newline(t_cstr arg)
{
	t_int	i;

	i = 0;
	if (arg[i] != '-')
		return (FALSE);
	i += 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (FALSE);
		i += 1;
	}
	return (TRUE);
}

t_int	builtin_echo(t_shell *sh, t_cmd *cmd)
{
	t_bool	print_newline;
	t_int	i;

	(void)sh;
	i = 1;
	print_newline = TRUE;
	while (cmd->args[i] && should_ignore_newline (cmd->args[i]))
	{
		print_newline = FALSE;
		i += 1;
	}
	while (i < cmd->args_count)
	{
		ft_print (cmd->args[i]);
		if (i != cmd->args_count - 1)
			ft_print (" ");
		i += 1;
	}
	if (print_newline)
		ft_print ("\n");
	return (0);
}

static t_bool	parse_exit_status(t_cstr arg, t_int *exit_status)
{
	t_s64	atoi_res;
	t_s64	i;

	i = 0;
	while (ft_is_space (arg[i]))
		i += 1;
	atoi_res = ft_str_to_int (arg + i, exit_status);
	if (atoi_res == 0)
		return (FALSE);
	i += atoi_res;
	while (arg[i])
	{
		if (!ft_is_space (arg[i]))
			return (FALSE);
		i += 1;
	}
	return (TRUE);
}

t_int	builtin_exit(t_shell *sh, t_cmd *cmd)
{
	t_int	exit_status;

	exit_status = 0;
	if (cmd->args_count > 2)
	{
		ft_fprintln (STDERR, "exit");
		eprint ("exit: too many arguments");
		return (1);
	}
	if (!cmd->next && !cmd->prev)
		ft_fprintln (STDERR, "exit");
	if (cmd->args_count > 1)
	{
		if (!parse_exit_status (cmd->args[1], &exit_status))
		{
			eprint ("exit: %s: numeric argument required", cmd->args[1]);
			exit_status = 255;
		}
	}
	if (!cmd->next && !cmd->prev)
		sh->should_exit = TRUE;
	return (exit_status);
}
