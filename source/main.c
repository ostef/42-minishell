/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:08:19 by aandric           #+#    #+#             */
/*   Updated: 2022/03/23 15:56:23 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parse_envp(t_shell *sh, t_str *envp)
{
	t_env	env;
	t_int	i;

	i = 0;
	while (envp[i])
	{
		ft_memset (&env, 0, sizeof (t_env));
		if (env_parse(envp[i], &env))
			env_set(sh, env.name, env.val);
		i++;
	}
}

static void	env_free(t_shell *sh)
{
	t_env	*next;

	while (sh->env_first)
	{
		next = sh->env_first->next;
		ft_free (sh->env_first->name, ft_heap ());
		ft_free (sh->env_first->val, ft_heap ());
		ft_free (sh->env_first, ft_heap ());
		sh->env_first = next;
		sh->env_count -= 1;
	}
}

static t_str	show_prompt(t_shell *sh)
{
	t_str	result;
	t_cstr	prompt;
	t_int	i;

	prompt = env_get (sh, "PWD");
	if (ft_strequ (prompt, env_get (sh, "HOME")))
		prompt = "~";
	else
	{
		i = ft_strlen (prompt);
		while (i > 0)
		{
			if (prompt[i - 1] == '/')
				break ;
			i -= 1;
		}
		prompt += i;
	}
	if (sh->last_exit_status == 0)
		prompt = ft_fmt (ft_temp (), "\033[0;1;32m%s$ \033[0m", prompt);
	else
		prompt = ft_fmt (ft_temp (), "\033[0;1;31m%s$ \033[0m", prompt);
	return readline (prompt);
}

t_int	main(t_int ac, t_str *av, t_str *envp)
{
	t_shell		sh;
	t_str		input;
	t_cmd_line	cmd_line;

	(void)ac;
	(void)av;
	ft_init_temp_storage ();
	ft_memset (&sh, 0, sizeof (t_shell));
	parse_envp (&sh, envp);
	while (!sh.should_exit)
	{
		ft_reset_temp_storage ();
		input = show_prompt(&sh);
		if (!input)
			break ;
		ft_memset (&cmd_line, 0, sizeof (t_cmd_line));
		if (cmd_line_parse (&sh, input, &cmd_line))
			sh.last_exit_status = cmd_line_exec (&sh, &cmd_line);
		free (input);
	}
	env_free (&sh);
	return (0);
}
