/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 15:07:04 by soumanso          #+#    #+#             */
/*   Updated: 2022/04/20 17:04:50 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static void	handle_dollars(t_shell *sh, t_lexer *lexer, t_builder *builder)
{
	t_token	*token;

	token = ft_lexer_skip_identifier (lexer);
	if (token)
		ft_builder_append (builder, env_get (sh,
				ft_strndup (token->str, token->len, ft_temp ())));
	else if (ft_lexer_skip_char (lexer, '?'))
		ft_builder_append_fmt (builder, "%d", g_globals.exit_status);
	else
		ft_builder_append_char (builder, '$');
}

static t_bool	handle_quote(t_lexer *lexer, t_builder *builder, char *quote)
{
	if (ft_lexer_skip_char(lexer, '\'') || ft_lexer_skip_char (lexer, '"'))
	{
		if (*quote == *(lexer->curr - 1))
			*quote = 0;
		else if (*quote == 0)
			*quote = *(lexer->curr - 1);
		else
			ft_builder_append_char (builder, *(lexer->curr - 1));
		return (TRUE);
	}
	return (FALSE);
}

t_str	expand_variables(t_shell *sh, t_cstr str, t_int len)
{
	t_lexer		lexer;
	t_builder	builder;
	char		quote;

	ft_builder_init (&builder, 100, ft_temp ());
	ft_lexer_init_n(&lexer, str, len, ft_temp());
	quote = 0;
	while (lexer.curr < lexer.end)
	{
		if (handle_quote(&lexer, &builder, &quote))
			continue ;
		if (quote != '\'' && ft_lexer_skip_char (&lexer, '$'))
			handle_dollars(sh, &lexer, &builder);
		else
		{
			ft_builder_append_char (&builder, *lexer.curr);
			lexer.curr += 1;
		}
	}
	return (ft_builder_build (&builder));
}

t_str	post_process_token(t_shell *sh, t_cstr str, t_int len)
{
	if (len == 0)
		return ("");
	if (str[0] == '~')
	{
		if (len > 1 && str[1] == '/')
			str = ft_fmt (ft_temp (), "%s%.*s",
					env_get (sh, "HOME"), len - 1, str + 1);
		else if (len == 1)
			str = ft_fmt (ft_temp (), "%s/%.*s",
					env_get (sh, "HOME"), len - 1, str + 1);
		len = ft_strlen (str);
	}
	return (expand_variables (sh, str, len));
}
