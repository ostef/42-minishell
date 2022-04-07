/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 14:02:40 by soumanso          #+#    #+#             */
/*   Updated: 2022/04/06 13:33:27 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cstr	parse_dir(t_shell *sh, t_cstr dir)
{
	if (ft_strequ (dir, "-"))
	{
		if (!env_get_node (sh, "OLDPWD"))
		{
			eprint ("cd: OLDPWD not set");
			return (NULL);
		}
		dir = env_get (sh, "OLDPWD");
	}
	else if (ft_strequ (dir, "--"))
	{
		if (!env_get_node (sh, "HOME"))
		{
			eprint ("cd: HOME not set");
			return (NULL);
		}
		dir = env_get (sh, "HOME");
	}
	return (dir);
}

static t_bool	check_dir(t_cstr dir)
{
	struct stat	stat_res;

	if (access (dir, F_OK) != 0 || stat (dir, &stat_res) != 0)
	{
		eprint ("cd: %s: %m", dir);
		return (FALSE);
	}
	if (!S_ISDIR (stat_res.st_mode))
	{
		errno = ENOTDIR;
		eprint ("cd: %s: %m", dir);
		return (FALSE);
	}
	return (TRUE);
}

t_int	builtin_cd(t_shell *sh, t_cmd *cmd)
{
	char	cwd_buff[PATH_MAX];
	t_cstr	new_dir;

	if (cmd->args_count < 2)
		return (EXIT_SUCCESS);
	new_dir = parse_dir (sh, cmd->args[1]);
	if (!new_dir || !check_dir (new_dir))
		return (EXIT_FAILURE);
	if (cmd->prev || cmd->next)
		return (EXIT_SUCCESS);
	if (chdir (new_dir) == -1)
	{
		eprint ("cd: %s: %m", new_dir);
		return (EXIT_FAILURE);
	}
	getcwd (cwd_buff, PATH_MAX);
	env_set (sh, "OLDPWD", env_get (sh, "PWD"));
	env_set (sh, "PWD", cwd_buff);
	return (EXIT_SUCCESS);
}

t_int	builtin_pwd(t_shell *sh, t_cmd *cmd)
{
	(void)cmd;
	ft_println (env_get (sh, "PWD"));
	return (EXIT_SUCCESS);
}
