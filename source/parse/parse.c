/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 16:10:35 by soumanso          #+#    #+#             */
/*   Updated: 2022/04/01 17:47:32 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static t_cmd	*cmd_add(t_cmd_line *line)
{
	t_cmd	*new;
	t_cmd	*tmp_next;

	new = (t_cmd *)ft_zalloc (sizeof (t_cmd), ft_temp ());
	if (!new)
		return (NULL);
	if (line->last)
	{
		tmp_next = line->last->next;
		line->last->next = new;
		new->prev = line->last;
		new->next = tmp_next;
		if (tmp_next)
			tmp_next->prev = new;
	}
	else
		line->first = new;
	line->last = new;
	line->count += 1;
	return (new);
}

static t_bool	cmd_parse(t_shell *sh, t_lexer *lexer, t_cmd *out)
{
	t_token			*token;
	t_redir_kind	redir_kind;

	while (lexer->curr < lexer->end)
	{
		ft_lexer_skip_spaces(lexer);
		redir_kind = cmd_parse_redir_symbol(lexer);
		if (lexer->curr < lexer->end && (*lexer->curr == '<' || *lexer->curr == '>'))
			return (FALSE);
		token = ft_lexer_skip_quoted_str(lexer);
		if (!token)
			token = ft_lexer_skip_delim(lexer, "\v\t\n\r |'\"");
		if (!token && redir_kind)
			return (FALSE);
		else if (!token)
			break ;
		if (token->len > 0)
		{
			token->str = post_process_token(sh, token->str, token->len);
			token->len = ft_strlen(token->str);
			if (redir_kind)
				cmd_add_redir (out, redir_kind, token);
			else
				cmd_add_arg (out, token);
		}
		if (token->kind == TK_DELIMITED && token->delim == '|')
			break ;
	}
	if (out->args_count == 0 && !out->redir_first)
		return (FALSE);
	return (cmd_null_terminate_args (out));
}

t_bool	cmd_line_parse(t_shell *sh, t_cstr str, t_cmd_line *line)
{
	t_lexer	lexer;
	t_cmd	*cmd;

	ft_lexer_init (&lexer, str, ft_temp());
	line->count = 0;
	while (lexer.curr < lexer.end)
	{
		cmd = cmd_add (line);
		if (!cmd)
			return (FALSE);
		if (!cmd_parse (sh, &lexer, cmd))
		{
			if (lexer.curr >= lexer.end)
				eprint ("syntax error near unexpected token `newline'");
			else
				eprint ("syntax error near unexpected token `%c'", *lexer.curr);
			sh->last_exit_status = 258;
			return (FALSE);
		}
		ft_lexer_skip_char (&lexer, '|');
		line->count += 1;
	}
	return (TRUE);
}
