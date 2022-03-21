/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 15:08:09 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/21 15:37:34 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "minishell.h"

t_str			post_process_token(t_shell *sh, t_cstr str, t_int len);
t_redir			*cmd_add_redir(t_cmd *cmd, t_redir_kind kind, t_token *token);
t_redir_kind	cmd_parse_redir_symbol(t_lexer *lexer);
t_bool			cmd_add_arg(t_cmd *cmd, t_token *token);
t_bool			cmd_null_terminate_args(t_cmd *cmd);

#endif
