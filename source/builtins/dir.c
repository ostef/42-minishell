/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 14:02:40 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/14 15:30:04 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_int	builtin_cd(t_shell *sh, t_cmd *cmd)
{
	char		cwd_buff[PATH_MAX];
	t_cstr		new_dir;
	struct stat	stat_res;

	if (cmd->args_count < 2 || cmd->prev || cmd->next)
		return (0);
	new_dir = cmd->args[1];
	if (chdir (new_dir) == -1)
	{
		eprint ("cd: %s: %m", new_dir);
		return (1);
	}
	getcwd (cwd_buff, PATH_MAX);
	env_set (sh, "OLDPWD", env_get (sh, "PWD"));
	env_set (sh, "PWD", cwd_buff);
	return (0);
}

t_int	builtin_pwd(t_shell *sh, t_cmd *cmd)
{
	ft_println (env_get (sh, "PWD"));
	return (0);
}
