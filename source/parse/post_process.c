/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 15:07:04 by soumanso          #+#    #+#             */
/*   Updated: 2022/04/17 17:04:35 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

t_str	expand_variables(t_shell *sh, t_cstr str, t_int len)
{
	t_lexer		lexer;
	t_token		*token;
	t_builder	builder;
	char		quote;

	ft_builder_init (&builder, 100, ft_temp ());
	ft_lexer_init_n(&lexer, str, len, ft_temp());
	quote = 0;
	while (lexer.curr < lexer.end)
	{
		if (ft_lexer_skip_char (&lexer, '\'') || ft_lexer_skip_char (&lexer, '"'))
		{
			if (quote == *(lexer.curr - 1))
				quote = 0;
			else if (quote == 0)
				quote = *(lexer.curr - 1);
			continue ;
		}
		if (quote != '\'' && ft_lexer_skip_char (&lexer, '$'))
		{
			token = ft_lexer_skip_identifier (&lexer);
			if (token)
				ft_builder_append (&builder, env_get (sh,
							ft_strndup (token->str, token->len, ft_temp ())));
			else if (ft_lexer_skip_char (&lexer, '?'))
				ft_builder_append_fmt (&builder, "%d", g_globals.exit_status);
			else
				ft_builder_append_char (&builder, '$');
		}
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
