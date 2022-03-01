/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 16:10:35 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/01 16:00:13 by soumanso         ###   ########lyon.fr   */
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

static t_s64	cmd_parse_word(t_cstr str, t_str *out, t_s64 *out_len)
{
	t_cstr	data;
	t_s64	len;
	t_s64	i;

	i = ft_skip_spaces (str);
	if (str[i] == '\'' || str[i] == '"')
	{
		i += 1;
		data = str + i;
		len = ft_skip_quoted_string (str + i, str[i - 1]) - 1;
		i += len + 1;
	}
	else
	{
		data = str + i;
		len = ft_skip_until_space (data);
		i += len;
	}
	if (out)
		*out = ft_strndup (data, len, ft_temp ());
	if (out_len)
		*out_len = len;
	return (i);
}

void	cmd_parse(t_cstr str, t_cmd *out)
{
	t_s64	i;
	t_s64	len;
	t_s64	start;
	t_s64	argi;

	i = cmd_parse_word (str, &out->name, NULL);
	out->arg_count = 0;
	start = i;
	while (str[i])
	{
		i += cmd_parse_word (str + i, NULL, &len);
		if (len != 0)
			out->arg_count += 1;
	}
	out->args = (t_str *)ft_zalloc (sizeof (t_str) * (out->arg_count + 1),
		ft_temp ());
	i = start;
	argi = 0;
	while (argi < out->arg_count)
	{
		i += cmd_parse_word (str + i, &out->args[argi], NULL);
		argi += 1;
	}
}

t_bool	cmd_line_parse(t_cstr str, t_cmd_line *out)
{
	(void)out;
	(void)str;
	return (TRUE);
}
