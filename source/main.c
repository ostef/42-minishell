/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 16:53:05 by soumanso          #+#    #+#             */
/*   Updated: 2022/02/12 16:26:13 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_str	get_prompt(void)
{
	static char	dir[PATH_MAX + 1];
	t_s64		dir_len;
	t_str		prompt;

	getcwd (dir, PATH_MAX + 1);
	dir_len = ft_strlen (dir);
	prompt = ft_alloc (PATH_MAX + 2, ALLOC_TEMP);
	ft_memcpy (prompt, dir, dir_len);
	prompt[dir_len] = PROMPT_CHAR;
	prompt[dir_len + 1] = 0;
	return (prompt);
}

int	main(int argc, t_str *args, t_str *env)
{
	t_cmd_line	line;

	(void)argc;
	(void)args;
	(void)env;
	ft_memset (&line, 0, sizeof (t_cmd_line));
	while (TRUE)
	{
		ft_reset_temp_storage ();
		line.str = readline (get_prompt ());
		if (!line.str || *line.str == 0)
			continue ;
		add_history (line.str);
		line.cmds = (t_cmd *)ft_alloc (sizeof (t_cmd) * 100, ALLOC_HEAP);
		parse_cmd_line (&line);
		execute_cmd_line (&line);
		free_cmd_line (&line);
	}
	if (ft_get_heap_allocations () != 0)
		ft_fprintln (STDERR, "Primitive leak checker found %i leaks.",
			ft_get_heap_allocations ());
	return (0);
}
