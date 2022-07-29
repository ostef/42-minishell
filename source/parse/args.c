/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 15:04:04 by soumanso          #+#    #+#             */
/*   Updated: 2022/07/29 21:17:02 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

t_bool	cmd_null_terminate_args(t_shell *sh, t_cmd *cmd)
{
	if (!cmd->args)
		return (TRUE);
	if (cmd->args_count == cmd->args_cap)
	{
		cmd->args = ft_realloc (cmd->args, cmd->args_cap * sizeof(t_str),
				(cmd->args_cap + cmd->args_cap + 8) * sizeof(t_str),
				sh->arena);
		if (!cmd->args)
			return (FALSE);
		cmd->args_cap += cmd->args_cap + 8;
	}
	cmd->args[cmd->args_count] = NULL;
	return (TRUE);
}

static void	reserve_enough_memory(t_shell *sh, t_cmd *cmd, t_int len)
{
	if (cmd->flat_args_count + len + 1 > cmd->flat_args_cap)
	{
		cmd->flat_args = ft_realloc (cmd->flat_args, cmd->flat_args_cap,
				cmd->flat_args_cap + cmd->flat_args_cap + len + 1,
				sh->arena);
		cmd->flat_args_cap += cmd->flat_args_cap + 8;
	}
	if (cmd->args_count == cmd->args_cap)
	{
		cmd->args = ft_realloc (cmd->args, cmd->args_cap * sizeof (t_str),
				(cmd->args_cap + cmd->args_cap + 8) * sizeof (t_str),
				sh->arena);
		cmd->args_cap += cmd->args_cap + 8;
	}
}

t_bool	cmd_add_arg(t_shell *sh, t_cmd *cmd, t_token *token)
{
	t_str	marker;

	reserve_enough_memory (sh, cmd, token->len);
	if (!cmd->flat_args || !cmd->args)
		return (FALSE);
	marker = cmd->flat_args + cmd->flat_args_count;
	ft_memcpy (marker, token->str, token->len);
	marker[token->len] = 0;
	cmd->flat_args_count += token->len + 1;
	cmd->args[cmd->args_count] = marker;
	cmd->args_count += 1;
	return (TRUE);
}
