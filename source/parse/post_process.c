/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 15:07:04 by soumanso          #+#    #+#             */
/*   Updated: 2022/04/06 13:35:27 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

t_str	expand_variables(t_shell *sh, t_cstr str, t_int len)
{
	t_lexer	lexer;
	t_token	*token;
	t_str	result;

	ft_lexer_init_n(&lexer, str, len, ft_temp());
	result = "";
	while (lexer.curr < lexer.end)
	{
		token = ft_lexer_skip_delim(&lexer, "$");
		result = ft_fmt(ft_temp(), "%s%.*s", result, token->len, token->str);
		ft_lexer_skip_char(&lexer, '$');
		token = ft_lexer_skip_char(&lexer, '?');
		if (token)
			result = ft_fmt(ft_temp(), "%s%d", result, g_exit_status);
		else
		{
			token = ft_lexer_skip_identifier(&lexer);
			if (token)
				result = ft_fmt(ft_temp(), "%s%s", result, env_get(sh,
							ft_strndup(token->str, token->len, ft_temp())));
		}
	}
	return (result);
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
