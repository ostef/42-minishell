/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:30:56 by aandric           #+#    #+#             */
/*   Updated: 2022/03/09 17:34:02 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	env_parse(t_cstr str, t_env *env)
{
	t_lexer lexer;
	t_token	*token;

	ft_lexer_init(&lexer, str, ft_temp());
	while (lexer.curr < lexer.end)
	{
		token = ft_lexer_skip_identifier(&lexer);
		if (!token)
			return (FALSE);
		env->name = ft_strndup(token->str, token->len, ft_temp());
		if (!ft_lexer_skip_char(&lexer, '='))
			return (FALSE);
		token = ft_lexer_skip_delim (&lexer, "\0");
		if (!token)
			return (FALSE);
		env->val = ft_strndup(token->str, token->len, ft_temp());
	}
	return (TRUE);
}

t_bool	env_set(t_shell *sh, t_cstr name, t_cstr val)
{
	t_env	*env_new;

	// @Todo (ana): Check if the variable already exists.
	env_new = ft_zalloc(sizeof(t_env), ft_heap());
	if (!env_new)
		return (FALSE);
	env_new->name = ft_strdup (name, ft_heap ());
	env_new->val = ft_strdup (val, ft_heap ());
	if (sh->env_last)
	{
		sh->env_last->next = env_new;
		env_new->prev = sh->env_last;
	}
	else
		sh->env_first = env_new;
	sh->env_last = env_new;
	return (TRUE);
}
