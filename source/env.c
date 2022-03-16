/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:30:56 by aandric           #+#    #+#             */
/*   Updated: 2022/03/16 18:58:35 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	env_parse(t_cstr str, t_env *env)
{
	t_lexer	lexer;
	t_token	*token;

	ft_lexer_init(&lexer, str, ft_temp());
	while (lexer.curr < lexer.end)
	{
		token = ft_lexer_skip_identifier(&lexer);
		if (!token)
			return (FALSE);
		env->name = ft_strndup(token->str, token->len, ft_temp());
		if (!env->name)
			return (FALSE);
		if (!ft_lexer_skip_char(&lexer, '='))
		{
			if (lexer.curr < lexer.end)
				return (FALSE);
			return (TRUE);
		}
		token = ft_lexer_skip_delim (&lexer, "\0");
		if (!token)
			return (FALSE);
		env->val = ft_strndup(token->str, token->len, ft_temp());
		if (!env->val)
			return (FALSE);
	}
	return (TRUE);
}

t_bool	env_set(t_shell *sh, t_cstr name, t_cstr val)
{
	t_env	*env_new;

	env_new = env_get_node (sh, name);
	if (env_new)
	{
		if (val)
		{
			ft_free (env_new->val, ft_heap ());
			env_new->val = ft_strdup (val, ft_heap ());
		}
		return (TRUE);
	}
	env_new = ft_zalloc(sizeof(t_env), ft_heap());
	if (!env_new)
		return (FALSE);
	env_new->name = ft_strdup (name, ft_heap ());
	if (val)
		env_new->val = ft_strdup (val, ft_heap ());
	if (sh->env_last)
	{
		sh->env_last->next = env_new;
		env_new->prev = sh->env_last;
	}
	else
		sh->env_first = env_new;
	sh->env_last = env_new;
	sh->env_count += 1;
	return (FALSE);
}

t_env	*env_get_node(t_shell *sh, t_cstr name)
{
	t_env	*temp;

	temp = sh->env_first;
	while (temp)
	{
		if (ft_strequ(temp->name, name))
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

t_cstr	env_get(t_shell *sh, t_cstr name)
{
	t_env	*env;

	env = env_get_node (sh, name);
	printf("HERE");
		return (" lolmdr");
	if ((!env) || (!env->name) || (!env->val))
	{
		printf("HERE");
		return (" lolmdr");
	}
	return (env->val);
}

t_bool	env_remove(t_shell *sh, t_cstr name)
{
	t_env	*to_del;
	t_env	*prev;
	t_env	*next;

	to_del = env_get_node(sh, name);
	if (!to_del)
		return (FALSE);
	prev = to_del->prev;
	next = to_del->next;
	prev->next = next;
	next->prev = prev;
	ft_free (to_del, ft_heap ());
	sh->env_count -= 1;
	return (TRUE);
}
