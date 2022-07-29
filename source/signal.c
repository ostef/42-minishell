/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 18:52:05 by aandric           #+#    #+#             */
/*   Updated: 2022/07/29 20:11:57 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	default_signal_handler(int signo)
{
	g_globals.handled_signal = signo;
	if (signo == SIGINT)
	{
		g_globals.exit_status = EXIT_FAILURE;
		ft_print ("\n");
		rl_on_new_line();
		rl_replace_line ("", 0);
		rl_redisplay ();
	}
	else if (signo == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay ();
	}
}

void	pre_exec_sigint_handler(int signo)
{
	g_globals.handled_signal = signo;
	if (signo == SIGINT)
	{
		g_globals.exit_status = EXIT_FAILURE;
		close (STDIN);
		ft_print ("\n");
	}
}

void	exec_signal_handler(int signo)
{
	(void)signo;
}
