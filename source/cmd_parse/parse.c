/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 16:10:35 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/02 17:02:58 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*cmd_add(t_cmd_line *line)
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

static t_bool	cmd_parse(t_lexer *lexer, t_cmd *out)
{
	t_token	*tk;
	t_int	i;

	(void)out;
	i = 0;
	while (lexer->curr < lexer->end)
	{
		ft_lexer_skip_spaces (lexer);
		tk = ft_lexer_skip_quoted_str (lexer);
		if (!tk)
			tk = ft_lexer_skip_delim (lexer, "\v\t\n\r |");
		if (!tk)
			break ;
		if (tk->len > 0)
			i += 1;
		if (tk->kind == TK_DELIMITED && tk->delim == '|')
			break ;
	}
	return (i != 0);
}

t_bool	cmd_line_parse(t_cstr str, t_cmd_line *line)
{
	t_lexer	lexer;
	t_cmd	*cmd;
	t_int	i;

	ft_lexer_init (&lexer, str, ft_temp ());
	i = 0;
	while (lexer.curr < lexer.end)
	{
		cmd = cmd_add (line);
		if (!cmd)
			return (FALSE);
		if (!cmd_parse (&lexer, cmd))
		{
			eprint ("syntax error near unexpected token `%c'", *lexer.curr);
			return (FALSE);
		}
		ft_lexer_skip_char (&lexer, '|');
		i += 1;
	}
	return (TRUE);
}
