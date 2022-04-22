/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 12:33:42 by aandric           #+#    #+#             */
/*   Updated: 2022/04/22 17:35:21 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_int	builtin_export_parse(t_shell *sh, t_cmd *cmd)
{
	if (cmd->args_count == 1)
		builtin_export(sh);
	if (cmd->args_count >= 2)
		return (builtin_export_add_val(sh, cmd));
	return (0);
}

t_int	builtin_export(t_shell *sh)
{
	t_env	*temp;

	temp = sh->env_first;
	while (temp)
	{
		if (!temp->val)
			ft_print("declare -x %s\n", temp->name);
		else
			ft_print("declare -x %s=\"%s\"\n", temp->name, temp->val);
		temp = temp->next;
	}
	return (0);
}

t_int	builtin_export_add_val(t_shell *sh, t_cmd *cmd)
{
	t_env	new_val;
	t_int	i;
	t_int	result;

	i = 1;
	result = 0;	
	while (cmd->args[i])
	{
		ft_memset(&new_val, 0, sizeof(t_env));
		if (env_parse(cmd->args[i], &new_val))
			env_set(sh, new_val.name, new_val.val);
		else
			result = 1;			
		i++;
	}
	return (result);
}

t_int	builtin_unset(t_shell *sh, t_cmd *cmd)
{
	t_int	i;

	i = 1;
	if (cmd->args_count == 1)
		return (TRUE);
	while (cmd->args[i])
	{
		env_remove(sh, cmd->args[i]);
		i++;
	}
	return (0);
}

t_int	builtin_env(t_shell *sh)
{
	t_env	*temp;

	temp = sh->env_first;
	while (temp)
	{
		if (temp->val)
			ft_print("%s=%s\n", temp->name, temp->val);
		temp = temp->next;
	}
	return (0);
}
