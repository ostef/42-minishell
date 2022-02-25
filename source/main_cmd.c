/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 13:51:12 by soumanso          #+#    #+#             */
/*   Updated: 2022/02/25 16:05:25 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*cmd_add(t_cmd_line *line)
{
	t_cmd	*new;
	t_cmd	*tmp_next;

	new = (t_cmd *)ft_alloc (sizeof (t_cmd), ft_temp ());
	if (!new)
		return (NULL);
	ft_memset (new, 0, sizeof (t_cmd));
	if (line->last)
	{
		tmp_next = line->last->next;
		line->last->next = new;
		new->prev = line->last;
		new->next = tmp_next;
		if (tmp_next)
			tmp_next->prev = new;
	}
	else
		line->first = new;
	line->last = new;
	line->count += 1;
	return (new);
}

void	parse_cmd(t_cstr str, t_cmd *out)
{
	t_s64	i;
	t_s64	start;
	t_s64	argi;

	i = ft_skip_spaces (str);
	start = i;
	i += ft_skip_until_space (str + i);
	out->name = ft_strndup (str + start, i - start, ft_temp ());
	start = i;
	out->arg_count = 0;
	while (str[i])
	{
		i += ft_skip_spaces (str + i);
		i += ft_skip_until_space (str + i);
		out->arg_count += 1;
	}
	i = start;
	out->args = (t_str *)ft_alloc (sizeof (t_str) * (out->arg_count + 1), ft_temp ());
	argi = 0;
	while (argi < out->arg_count)
	{
		i += ft_skip_spaces (str + i);
		start = i;
		i += ft_skip_until_space (str + i);
		out->args[argi] = ft_strndup (str + start, i - start, ft_temp ());
		argi += 1;
	}
	out->args[out->arg_count] = NULL;
}

t_int	main(t_int argc, t_str *args, t_str *env)
{
	t_shell		sh;
	t_cmd_line	line;
	t_cmd		*cmd;
	t_int		i;
	t_int		status;

	ft_init_temp_storage ();
	ft_memset (&sh, 0, sizeof (t_shell));
	ft_memset (&line, 0, sizeof (t_cmd_line));
	sh.env_original = env;
	i = 1;
	while (i < argc)
	{
		
		cmd = cmd_add (&line);
		ft_assert (cmd != NULL, "Could not create cmd for '%s'.", args[i]);
		parse_cmd (args[i], cmd);
		i += 1;
	}
	status = cmd_line_exec (&sh, &line);
	ft_println ("Last command exitted with code: %i.", status);
}
