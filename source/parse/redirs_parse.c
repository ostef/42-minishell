/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 15:04:35 by soumanso          #+#    #+#             */
/*   Updated: 2022/07/29 21:43:13 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

t_redir_kind	cmd_parse_redir_symbol(t_lexer *lexer)
{
	t_token			*token;
	t_redir_kind	kind;

	token = ft_lexer_skip_string (lexer, "<<");
	kind = RD_IN_HERE;
	if (!token)
	{
		token = ft_lexer_skip_string (lexer, "<");
		kind = RD_IN;
	}
	if (!token)
	{
		token = ft_lexer_skip_string (lexer, ">>");
		kind = RD_OUT_APPEND;
	}
	if (!token)
	{
		token = ft_lexer_skip_string (lexer, ">");
		kind = RD_OUT;
	}
	if (!token)
		return (RD_NONE);
	ft_lexer_skip_spaces (lexer);
	return (kind);
}

t_redir	*cmd_add_redir(t_shell *sh, t_cmd *cmd,
	t_redir_kind kind, t_token *token)
{
	t_redir	*new;
	t_redir	*tmp_next;

	new = (t_redir *)ft_zalloc (sizeof (t_redir), sh->arena);
	if (!new)
		return (NULL);
	new->kind = kind;
	new->filename = ft_strndup (token->str, token->len, sh->arena);
	if (cmd->redir_last)
	{
		tmp_next = cmd->redir_last->next;
		cmd->redir_last->next = new;
		new->prev = cmd->redir_last;
		new->next = tmp_next;
		if (tmp_next)
			tmp_next->prev = new;
	}
	else
		cmd->redir_first = new;
	cmd->redir_last = new;
	return (new);
}
