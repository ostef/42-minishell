/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 18:08:19 by aandric           #+#    #+#             */
/*   Updated: 2022/07/29 21:36:20 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_int	main(t_int ac, t_str *av, t_str *envp)
{
	t_shell	sh;

	(void)ac;
	(void)av;
	ft_memset (&sh, 0, sizeof (t_shell));
	ft_init_arena (&sh.arena_memory, TEMP_STORAGE_SIZE);
	sh.arena = ft_arena (&sh.arena_memory);
	tcgetattr(0, &sh.old_termios);
	sh.new_termios = sh.old_termios;
	sh.new_termios.c_lflag &= ~ECHOCTL;
	env_init (&sh, envp);
	while (!sh.should_exit)
		shell_loop (&sh);
	env_free (&sh);
	tcsetattr(0, TCSANOW, &sh.old_termios);
	ft_free_arena (&sh.arena_memory);
	if (ft_get_heap_allocations () != 0)
		eprint ("Found %i leaks.", ft_get_heap_allocations ());
	return (g_globals.exit_status);
}
