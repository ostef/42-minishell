/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:30:56 by aandric           #+#    #+#             */
/*   Updated: 2022/07/29 21:21:46 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool	parse_ident(t_shell *sh, t_lexer *lexer, t_env *env)
{
	t_token	*token;
	t_s64	i;

	token = ft_lexer_skip_delim(lexer, "=");
	if (!token || token->len == 0)
		return (FALSE);
	i = 0;
	while (i < token->len)
	{
		if (i == 0 && !ft_is_alpha (token->str[i]) && token->str[i] != '_')
			return (FALSE);
		if (!ft_is_alnum (token->str[i]))
			return (FALSE);
		i += 1;
	}
	env->name = ft_strndup(token->str, token->len, sh->arena);
	if (!env->name)
		return (FALSE);
	return (TRUE);
}

static t_bool	parse_val(t_shell *sh, t_lexer *lexer, t_env *env)
{
	t_token	*token;

	if (lexer->curr >= lexer->end)
	{
		env->val = ft_strdup ("", sh->arena);
		return (TRUE);
	}
	token = ft_lexer_skip_delim (lexer, "\0");
	if (!token)
		return (FALSE);
	env->val = ft_strndup(token->str, token->len, sh->arena);
	if (!env->val)
		return (FALSE);
	return (TRUE);
}

t_bool	env_parse(t_shell *sh, t_cstr str, t_env *env)
{
	t_lexer	lexer;
	t_token	*token;
	t_bool	ok;

	ft_lexer_init(&lexer, str, sh->arena);
	if (lexer.curr == lexer.end)
		return (eprint("export: `': not a valid identifier"));
	while (lexer.curr < lexer.end)
	{
		ok = parse_ident (sh, &lexer, env);
		if (!ok)
		{
			token = lexer.last_token;
			if (token)
				return (eprint ("export: `%*s': not a valid identifier",
						token->len, token->str));
			else
				return (eprint ("export: `': not a valid identifier"));
		}
		if (!ft_lexer_skip_char(&lexer, '='))
			return (lexer.curr == lexer.end);
		if (!parse_val (sh, &lexer, env))
			return (eprint ("export: Could not parse value"));
	}
	return (TRUE);
}
