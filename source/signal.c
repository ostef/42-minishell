/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 18:52:05 by aandric           #+#    #+#             */
/*   Updated: 2022/03/31 17:06:01 by aandric          ###   ########lyon.fr   */
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
	//char str[6] = "\bexit";
	write(2, "la", 2);
	//write(2, "\n", 1);
	//write(2, "exit\n", 5);
	rl_on_new_line(); // Regenerate the prompt on a newline
	rl_replace_line("exit", 0); // Clear the previous text
	rl_redisplay();
	(void)signo;
}

void	sig_handler(void)
{
	signal(SIGINT, int_handler);
	signal(SIGQUIT, silence);
	//signal(SIGKILL, silence);
	//signal()
}
