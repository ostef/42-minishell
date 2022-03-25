/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 15:33:04 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/21 16:09:37 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_int	builtin_echo(t_shell *sh, t_cmd *cmd)
{
	t_bool	print_newline;
	t_int	i;

	i = 1;
	print_newline = TRUE;
	if (cmd->args_count > 1 && ft_strequ(cmd->args[1], "-n"))
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

t_int	builtin_exit(t_shell *sh, t_cmd *cmd)
{
	if (!cmd->next && !cmd->prev)
		sh->should_exit = TRUE;
	return (0);
}