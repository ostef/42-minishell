/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:08:19 by aandric           #+#    #+#             */
/*   Updated: 2022/04/01 19:18:26 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_globals	g_globals;

static void	env_init(t_shell *sh, t_str *envp)
{
	t_env	env;
	t_int	i;
	char	cwd[PATH_MAX];
	t_int	shlvl;

	i = 0;
	while (envp[i])
	{
		ft_memset (&env, 0, sizeof (t_env));
		if (env_parse(envp[i], &env))
			env_set(sh, env.name, env.val);
		i++;
	}
	getcwd (cwd, PATH_MAX);
	env_set (sh, "PWD", cwd);
	env_set (sh, "OLDPWD", NULL);
	ft_str_to_int (env_get (sh, "SHLVL"), &shlvl);
	env_set (sh, "SHLVL", ft_fmt (ft_temp (), "%i", shlvl + 1));
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

static t_cstr	get_prompt(t_shell *sh)
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
	if (sh->last_exit_status == 0)
		prompt = ft_fmt (ft_temp (), "\033[0;1;32m%s$ \033[0m", prompt);
	else
		prompt = ft_fmt (ft_temp (), "\033[0;1;31m%s$ \033[0m", prompt);
	return (prompt);
}

void	set_termios(t_shell *sh)
{
	tcgetattr(0, &sh->old_termios);
	sh->new_termios = sh->old_termios;
	sh->new_termios.c_lflag &= ~ECHOCTL;
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
	set_termios(&sh);
	env_init (&sh, envp);
	while (!sh.should_exit)
	{
		tcsetattr(0, TCSANOW, &sh.new_termios);
		sig_handler();
		ft_reset_temp_storage ();
		signal(SIGINT, int_handler);
		ft_print ("\033[s");
		input = readline (get_prompt(&sh));
		signal(SIGINT, silence);
		if (!input)
		{
			ft_println ("\033[u%sexit", get_prompt (&sh));
			break ;
		}
		else
			add_history(input);
		ft_memset (&cmd_line, 0, sizeof (t_cmd_line));
		if (cmd_line_parse (&sh, input, &cmd_line))
			sh.last_exit_status = cmd_line_exec (&sh, &cmd_line);
		free (input);
	}
	env_free (&sh);
	tcsetattr(0, TCSANOW, &sh.old_termios);
	return (0);
}
