/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 18:49:39 by soumanso          #+#    #+#             */
/*   Updated: 2022/04/04 18:52:24 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	env_add_node(t_shell *sh, t_env *node)
{
	if (sh->env_last)
	{
		sh->env_last->next = node;
		node->prev = sh->env_last;
	}
	else
		sh->env_first = node;
	sh->env_last = node;
	sh->env_count += 1;
}

t_bool	env_set(t_shell *sh, t_cstr name, t_cstr val)
{
	t_env	*node;

	node = env_get_node (sh, name);
	if (node)
	{
		if (val)
		{
			ft_free (node->val, ft_heap ());
			node->val = ft_strdup (val, ft_heap ());
		}
		return (TRUE);
	}
	node = (t_env *)ft_zalloc(sizeof(t_env), ft_heap());
	if (!node)
		return (FALSE);
	node->name = ft_strdup (name, ft_heap ());
	if (val)
		node->val = ft_strdup (val, ft_heap ());
	env_add_node (sh, node);
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
	if (!env && ft_strequ (name, "PATH"))
		return (DEF_PATH);
	if (!env || !env->val)
		return ("");
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