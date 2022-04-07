/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 18:52:05 by aandric           #+#    #+#             */
/*   Updated: 2022/04/06 13:49:17 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	default_signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_exit_status = 1;
		ft_print ("\n");
		rl_on_new_line();
		ft_print ("\033[s");
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signo == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay ();
	}
}

void	pre_exec_sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
		close (STDIN);
		ft_print ("\n");
	}
}

void	exec_signal_handler(int signo)
{
	if (signo == SIGINT)
		ft_print ("\n");
	else if (signo == SIGQUIT)
		ft_println ("Quit: %i", signo);
}
