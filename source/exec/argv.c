/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 15:57:07 by soumanso          #+#    #+#             */
/*   Updated: 2022/02/28 16:04:11 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_s64	cmd_get_argv_buff_len(t_cmd *cmd)
{
	t_s64	buff_len;
	t_s64	i;

	buff_len = ft_strlen (cmd->name) + 1;
	i = 0;
	while (cmd->args[i])
	{
		buff_len += ft_strlen (cmd->name) + 1;
		i += 1;
	}
	return (buff_len);
}

/* Returns all the arguments joined together, separated by \0s */
static t_str	cmd_get_argv_buff(t_cmd *cmd)
{
	t_s64	buff_len;
	t_str	buff;
	t_s64	offset;
	t_s64	i;

	buff_len = cmd_get_argv_buff_len (cmd);
	buff = (t_str)ft_alloc (buff_len, ft_temp ());
	if (!buff)
		return (NULL);
	ft_memcpy (buff, cmd->name, ft_strlen (cmd->name) + 1);
	offset = ft_strlen (cmd->name) + 1;
	i = 0;
	while (cmd->args[i])
	{
		ft_memcpy (buff + offset, cmd->args[i], ft_strlen (cmd->args[i]) + 1);
		offset += ft_strlen (cmd->args[i]) + 1;
		i += 1;
	}
	return (buff);
}

t_str	*cmd_get_argv(t_cmd *cmd)
{
	t_str	buff;
	t_str	*result;
	t_s64	i;

	buff = cmd_get_argv_buff (cmd);
	if (!buff)
		return (NULL);
	result = (t_str *)ft_alloc (
		sizeof (t_str) * (cmd->arg_count + 2), ft_temp ());
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
