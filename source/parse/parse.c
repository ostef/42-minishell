/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 16:10:35 by soumanso          #+#    #+#             */
/*   Updated: 2022/04/20 17:22:51 by aandric          ###   ########lyon.fr   */
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

static t_token	*parse_word(t_shell *sh, t_lexer *lexer,
	t_redir_kind redir, t_cmd *out)
{
	t_token	*token;
	char	quote;

	if (lexer->curr >= lexer->end)
		return (NULL);
	token = ft_lexer_push_token (lexer);
	if (!token)
		return (NULL);
	token->str = lexer->curr;
	quote = 0;
	while (lexer->curr < lexer->end)
	{
		if (*lexer->curr == '"' || *lexer->curr == '\'')
		{
			if (quote == *lexer->curr)
				quote = 0;
			else if (quote == 0)
				quote = *lexer->curr;
		}
		if (quote == 0 && ft_strchr ("\v\t\n\r <>|", *lexer->curr))
			break ;
		lexer->curr += 1;
	}
	token->len = lexer->curr - token->str;
	if (token->len > 0)
	{
		if (redir != RD_IN_HERE)
		{
			token->str = post_process_token(sh, token->str, token->len);
			token->len = ft_strlen(token->str);
		}
		if (redir)
			cmd_add_redir (out, redir, token);
		else
			cmd_add_arg (out, token);
	}
	return (token);
}

static t_bool	cmd_parse(t_shell *sh, t_lexer *lexer, t_cmd *out)
{
	t_token			*token;
	t_redir_kind	redir_kind;

	while (lexer->curr < lexer->end)
	{
		ft_lexer_skip_spaces(lexer);
		redir_kind = cmd_parse_redir_symbol(lexer);
		if (lexer->curr < lexer->end
			&& (*lexer->curr == '<' || *lexer->curr == '>'))
			return (FALSE);
		token = parse_word (sh, lexer, redir_kind, out);
		if (!token && redir_kind)
			return (FALSE);
		if (!token || *lexer->curr == '|')
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
			g_globals.exit_status = EXIT_SYNTAX_ERROR;
			if (lexer.curr >= lexer.end)
				return (eprint ("%s `newline'", ERR_STR_SYNTAX));
			else
				return (eprint ("%s `%c'", ERR_STR_SYNTAX, *lexer.curr));
		}
		if (ft_lexer_skip_char (&lexer, '|') && lexer.curr == lexer.end)
			return (eprint ("%s `newline'", ERR_STR_SYNTAX));
	}
	return (TRUE);
}
