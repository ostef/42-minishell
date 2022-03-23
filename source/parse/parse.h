/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:redir.c
/*   Created: 2022/03/18 17:17:40 by aandric           #+#    #+#             */
/*   Updated: 2022/03/18 19:23:04 by aandric          ###   ########lyon.fr   */
=======
/*   Created: 2022/03/21 15:08:09 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/21 15:37:34 by soumanso         ###   ########lyon.fr   */
>>>>>>> 82259dd9363502f33ccbecd0723302994e61fc55:source/parse/parse.h
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

<<<<<<< HEAD:redir.c

t_bool	ft_redir(t_shell *shell, t_cmd *cmd)
{
	if (!cmd->first_redir)
		return (0);
	if (kind == RD_NONE)
	return (1);
}

t_bool ft_redir_out(t_shell *shell, t_cmd *cmd)
{
	int	fd_in;
	int	fd_out;

	fd_in = open(cmd->first_redir->filename, O_RDONLY);
	if (fd_in < 0)
		return (0);
	fd_out = open(cmd->last_redir->filename, O_WRONLY);
	if (fd_out < 0)
		return (0);
	dup2(fd_in, fd_out);
}
=======
# include "minishell.h"

t_str			post_process_token(t_shell *sh, t_cstr str, t_int len);
t_redir			*cmd_add_redir(t_cmd *cmd, t_redir_kind kind, t_token *token);
t_redir_kind	cmd_parse_redir_symbol(t_lexer *lexer);
t_bool			cmd_add_arg(t_cmd *cmd, t_token *token);
t_bool			cmd_null_terminate_args(t_cmd *cmd);

#endif
>>>>>>> 82259dd9363502f33ccbecd0723302994e61fc55:source/parse/parse.h
