/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 15:32:22 by soumanso          #+#    #+#             */
/*   Updated: 2022/07/29 21:19:46 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static t_bool	is_path(t_cstr filename)
{
	return (filename[0] == '.' || filename[0] == '/');
}

static t_err	is_command(t_cstr filename)
{
	struct stat	stat_res;

	if (access (filename, F_OK) != 0)
		return (ERR_CMD_NO_SUCH_FILE);
	if (stat (filename, &stat_res) == 0)
	{
		if (S_ISDIR (stat_res.st_mode))
			return (ERR_CMD_IS_DIR);
		else if (access (filename, X_OK) != 0)
			return (ERR_CMD_PERM);
	}
	else if (errno == EACCES)
		return (ERR_CMD_PERM);
	else
		return (ERR_CMD_NOT_FOUND);
	return (OK);
}

t_err	cmd_find_path(t_shell *sh, t_cstr cmd_name, t_cstr *out_filename)
{
	t_lexer	lexer;
	t_token	*token;

	*out_filename = cmd_name;
	if (is_path (*out_filename))
		return (is_command (*out_filename));
	if (!env_get (sh, "PATH"))
		return (ERR_CMD_NOT_FOUND);
	ft_lexer_init (&lexer, env_get (sh, "PATH"), sh->arena);
	while (lexer.curr < lexer.end)
	{
		token = ft_lexer_skip_delim (&lexer, ":");
		if (!token)
			break ;
		ft_lexer_skip_char (&lexer, ':');
		*out_filename = ft_fmt(sh->arena, "%.*s/%s",
				token->len, token->str, cmd_name);
		if (is_command (*out_filename) == OK)
			return (OK);
	}
	return (ERR_CMD_NOT_FOUND);
}
