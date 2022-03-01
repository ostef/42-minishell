/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:08:19 by aandric           #+#    #+#             */
/*   Updated: 2022/03/01 15:53:48 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int main (int ac, char **av)
{
	t_shell		sh;
	t_str		line;
	t_cmd_line	cmd_line;

	(void)ac;
	(void)av;
	ft_init_temp_storage ();
	ft_memset (&sh, 0, sizeof (t_shell));
	while (TRUE)
	{
		ft_reset_temp_storage ();
		line = readline("$");
		ft_memset (&cmd_line, 0, sizeof (t_cmd_line));
		if (cmd_line_parse (line, &cmd_line))
			sh.last_exit_code = cmd_line_exec (&sh, &cmd_line);
		free (line);
	}
	return (0);
}