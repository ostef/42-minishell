/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 18:52:05 by aandric           #+#    #+#             */
/*   Updated: 2022/04/01 18:22:49 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	int_handler(int signo)
{
	write(1, "\n", 1);
	rl_on_new_line(); // Regenerate the prompt on a newline
	rl_replace_line("", 0); // Clear the previous text
	rl_redisplay();
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
