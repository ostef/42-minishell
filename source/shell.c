/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 21:00:39 by soumanso          #+#    #+#             */
/*   Updated: 2022/07/31 13:15:51 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_globals	g_globals;

void	env_init(t_shell *sh, t_str *envp)
{
	t_env	env;
	t_int	i;
	char	cwd[PATH_MAX];
	t_int	shlvl;

	i = 0;
	while (envp[i])
	{
		ft_memset (&env, 0, sizeof (t_env));
		if (env_parse(sh, envp[i], &env))
			env_set(sh, env.name, env.val);
		i++;
	}
	getcwd (cwd, PATH_MAX);
	env_set (sh, "PWD", cwd);
	env_remove (sh, "OLDPWD");
	env_set (sh, "OLDPWD", NULL);
	ft_str_to_int (env_get (sh, "SHLVL"), &shlvl);
	env_set (sh, "SHLVL", ft_fmt (sh->arena, "%i", shlvl + 1));
	if (!env_get_node (sh, "PATH"))
	{
		env_set (sh, "PATH", DEF_PATH);
		env_get_node (sh, "PATH")->hide = TRUE;
	}
}

void	env_free(t_shell *sh)
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

t_cstr	get_prompt(t_shell *sh)
{
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
	return (ft_fmt (sh->arena, "\033[1m%s$ \033[0m", prompt));
}

static t_bool	should_be_added_to_history(t_cstr line)
{
	t_int	i;

	i = 0;
	while (line[i])
	{
		if (!ft_is_space (line[i]))
			return (TRUE);
		i += 1;
	}
	return (FALSE);
}

void	shell_loop(t_shell *sh)
{
	t_str		input;
	t_cmd_line	line;

	ft_reset_arena (&sh->arena_memory);
	tcsetattr(0, TCSANOW, &sh->new_termios);
	g_globals.handled_signal = 0;
	signal (SIGINT, default_signal_handler);
	signal (SIGQUIT, default_signal_handler);
	input = readline (get_prompt(sh));
	if (!input)
	{
		g_globals.exit_status = 1;
		ft_println ("exit");
		sh->should_exit = TRUE;
		return ;
	}
	else if (should_be_added_to_history (input))
		add_history(input);
	ft_memset (&line, 0, sizeof (t_cmd_line));
	if (cmd_line_parse (sh, input, &line))
		g_globals.exit_status = cmd_line_exec (sh, &line);
	free (input);
}
