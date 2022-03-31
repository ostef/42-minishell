/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 18:52:05 by aandric           #+#    #+#             */
/*   Updated: 2022/03/30 19:07:10 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_nl(void)
{
	rl_on_new_line(); // Regenerate the prompt on a newline
	rl_replace_line("\n", 0); // Clear the previous text
	rl_redisplay();
}

void silence(int signo)
{
	(void)signo;
}

void	sig_handler(void)
{
	signal(SIGINT, sig_nl);
	signal(SIGQUIT, silence);
}
