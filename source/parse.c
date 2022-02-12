/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 17:09:47 by soumanso          #+#    #+#             */
/*   Updated: 2022/02/11 17:44:22 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_parser
{
	t_str	start;
	t_str	end;
	t_str	curr;
}	t_parser;

static void	skip_spaces (t_parser *parser)
{
	while (parser->curr < parser->end && ft_is_space (*parser->curr))
		parser->curr += 1;
}

static t_str	parse_until_space(t_parser *parser)
{
	t_str	start;

	start = parser->curr;
	while (parser->curr < parser->end && !ft_is_space (*parser->curr))
		parser->curr += 1;
	if (parser->curr == start)
		return (NULL);
	return (ft_strndup (start, parser->curr - start, ALLOC_HEAP));
}

t_bool	parse_cmd_line(t_cmd_line *line)
{
	t_parser	parser;
	t_cmd		cmd;

	parser.start = line->str;
	parser.end = parser.start + ft_strlen (line->str);
	parser.curr = line->str;
	while (parser.curr < parser.end)
	{
		skip_spaces (&parser);
		cmd.name_or_path = parse_until_space (&parser);
		cmd.arg_count = 0;
		cmd.args = (t_str *)ft_alloc (sizeof (t_str) * 100, ALLOC_HEAP);
		while (parser.curr < parser.end)
		{
			skip_spaces (&parser);
			cmd.args[cmd.arg_count] = parse_until_space (&parser);
			if (cmd.args[cmd.arg_count])
				cmd.arg_count += 1;
		}
		line->cmds[line->cmd_count] = cmd;
		line->cmd_count += 1;
	}
	return (TRUE);
}
