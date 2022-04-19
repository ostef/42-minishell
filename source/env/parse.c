/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:30:56 by aandric           #+#    #+#             */
/*   Updated: 2022/04/15 12:35:52 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	parse_ident(t_lexer *lexer, t_env *env)
{
	t_token	*token;

	token = ft_lexer_skip_identifier(lexer);
	if (!token)
		return (FALSE);
	env->name = ft_strndup(token->str, token->len, ft_temp());
	if (!env->name)
		return (FALSE);
	return (TRUE);
}

static t_bool	parse_val(t_lexer *lexer, t_env *env)
{
	t_token	*token;

	token = ft_lexer_skip_delim (lexer, "\0");
	if (!token)
		return (FALSE);
	env->val = ft_strndup(token->str, token->len, ft_temp());
	if (!env->val)
		return (FALSE);
	return (TRUE);
}

static void	ft_print_error_env(t_cstr str)
{
	t_int	i;

	i = 0;
	while (str[i])
	{
		ft_fprint(2, "export: '");
		if (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
			i++;
		else
		{
			write(2, &str[i], 1);
			if (str[i] == '=')
				break ;
			i++;
		}
	}
	ft_fprint(2, "' :not a valid identifier\n");
}

t_bool	env_parse(t_cstr str, t_env *env)
{
	t_lexer	lexer;

	ft_lexer_init(&lexer, str, ft_temp());
	while (lexer.curr < lexer.end)
	{
		if (!parse_ident (&lexer, env))
		{
			ft_print_error_env(str);
			return (FALSE);
		}
		if (!ft_lexer_skip_char(&lexer, '='))
			return (lexer.curr == lexer.end);
		if (!parse_val (&lexer, env))
			return (FALSE);
	}
	return (TRUE);
}
