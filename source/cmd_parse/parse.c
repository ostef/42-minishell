/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 16:10:35 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/02 16:21:54 by soumanso         ###   ########lyon.fr   */
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

static void	cmd_parse(t_lexer *lexer, t_cmd *out)
{
	t_token	*tk;

	(void)out;
	while (lexer->curr < lexer->end)
	{
		ft_lexer_skip_spaces (lexer);
		tk = ft_lexer_skip_quoted_str (lexer);
		if (!tk)
			tk = ft_lexer_skip_delim (lexer, "\v\t\n\r |");
		if (!tk)
			break ;
		if (tk->len > 0)
			ft_println ("%.*s", tk->len, tk->str);
		if (tk->kind == TK_DELIMITED && tk->delim == '|')
			break ;
	}
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
		ft_println ("cmd [%i]:", i);
		cmd = cmd_add (line);
		if (!cmd)
			return (FALSE);
		cmd_parse (&lexer, cmd);
		ft_lexer_skip_char (&lexer, '|');
		i += 1;
	}
	return (TRUE);
}
