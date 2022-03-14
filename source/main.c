/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:08:19 by aandric           #+#    #+#             */
/*   Updated: 2022/03/14 19:00:14 by soumanso         ###   ########lyon.fr   */
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
	while (TRUE)
	{
		ft_reset_temp_storage ();
		input = readline("minishell$ ");
		if (!input)
			break ;
		ft_memset (&cmd_line, 0, sizeof (t_cmd_line));
		if (cmd_line_parse (&sh, input, &cmd_line))
		{
			sh.last_exit_status = cmd_line_exec (&sh, &cmd_line);
			ft_println ("$? = %i", sh.last_exit_status);
		}
		free (input);
	}
	return (0);
}
