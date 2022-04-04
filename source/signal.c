/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 18:52:05 by aandric           #+#    #+#             */
/*   Updated: 2022/04/03 20:43:51 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	int_handler(int signo)
{
	ft_print ("\n");
	rl_on_new_line(); // Regenerate the prompt on a newline
	ft_print ("\033[s");
	rl_replace_line("", 0); // Clear the previous text
	rl_redisplay();
}

void	exec_int_handler(int signo)
{
	close (STDIN);
	g_globals.exit_exec = TRUE;
	ft_print ("\n");
}

void silence(int signo)
{
	rl_on_new_line(); // Regenerate the prompt on a newline
	rl_redisplay();
	(void)signo;
}

void put_nl(int signo)
{
	write(1, "\n", 1);
	(void)signo;
}

void	quit_3(int signo)
{
	ft_println("Quit: %i", signo);
}

void	sig_handler(void)
{
	signal(SIGINT, int_handler);
	signal(SIGQUIT, silence);
}
