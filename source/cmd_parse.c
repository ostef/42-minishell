/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 16:10:35 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/11 17:10:39 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*cmd_add(t_cmd_line *line)
{
	t_cmd	*new;
	t_cmd	*tmp_next;

	new = (t_cmd *)ft_zalloc (sizeof (t_cmd), ft_temp ());
	if (!new)
		return (NULL);
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

static t_bool	cmd_null_terminate_args(t_cmd *cmd)
{
	if (cmd->args_count == cmd->args_cap)
	{
		cmd->args = ft_realloc (cmd->args, cmd->args_cap * sizeof (t_str),
				(cmd->args_cap + cmd->args_cap + 8) * sizeof (t_str), ft_temp ());
		if (!cmd->args)
			return (FALSE);
		cmd->args_cap += cmd->args_cap + 8;
	}
	cmd->args[cmd->args_count] = NULL;
	return (TRUE);
}

static t_bool	cmd_add_arg(t_cmd *cmd, t_cstr arg, t_s64 arg_len)
{
	t_str	marker;

	if (cmd->flat_args_count + arg_len + 1 > cmd->flat_args_cap)
	{
		cmd->flat_args = ft_realloc (cmd->flat_args, cmd->flat_args_cap,
			cmd->flat_args_cap + cmd->flat_args_cap + arg_len + 1, ft_temp ());
		cmd->flat_args_cap += cmd->flat_args_cap + 8;
	}
	if (cmd->args_count == cmd->args_cap)
	{
		cmd->args = ft_realloc (cmd->args, cmd->args_cap * sizeof (t_str),
			(cmd->args_cap + cmd->args_cap + 8) * sizeof (t_str), ft_temp ());
		cmd->args_cap += cmd->args_cap + 8;
	}
	if (!cmd->flat_args || !cmd->args)
		return (FALSE);
	marker = cmd->flat_args + cmd->flat_args_count;
	ft_memcpy (marker, arg, arg_len);
	marker[arg_len] = 0;
	cmd->flat_args_count += arg_len + 1;
	cmd->args[cmd->args_count] = marker;
	cmd->args_count += 1;
	return (TRUE);
}

t_cstr	replace_dollars(t_shell *sh, t_cstr str, t_int len)
{
	t_lexer	lexer;
	t_token	*token;
	t_str	result;
	t_int	nb_dollars;
	
	ft_lexer_init_n(&lexer, str, len, ft_temp());
	nb_dollars = ft_count_chars(str, '$', len);
	if (nb_dollars < 1 || str[len - 1] == '$')
		return (str);
	while (nb_dollars && lexer.curr < lexer.end)
	{
		ft_lexer_skip_delim(&lexer, "$");
		ft_lexer_skip_char(&lexer, '$');
		token = ft_lexer_skip_identifier(&lexer);
		if ((token->str)[0] == '$')
			return (str);
		
		ft_println ("J'ai trouve un dollar! %.*s", token->len, token->str);
		nb_dollars--;
	}
	result = ft_strndup(token->str, token->len, ft_temp());
	return (result);
	//env_get_node(sh, str);
}


t_str	string_replace(t_str to_find, t_str to_replace)
{
	
}


static t_bool	cmd_parse(t_shell *sh, t_lexer *lexer, t_cmd *out)
{
	t_token	*token;

	while (lexer->curr < lexer->end)
	{
		ft_lexer_skip_spaces (lexer);
		token = ft_lexer_skip_quoted_str (lexer);
		if (!token)
			token = ft_lexer_skip_delim (lexer, "\v\t\n\r |");
		if (!token)
			break ;
		if (token->len > 0)
		{
			replace_dollars(sh, token->str, token->len);
			cmd_add_arg (out, token->str, token->len);
		}
		if (token->kind == TK_DELIMITED && token->delim == '|')
			break ;
	}
	if (out->args_count == 0)
		return (FALSE);
	return (cmd_null_terminate_args (out));
}

t_bool	cmd_line_parse(t_shell *sh, t_cstr str, t_cmd_line *line)
{
	t_lexer	lexer;
	t_cmd	*cmd;

	ft_lexer_init (&lexer, str, ft_temp());
	line->count = 0;
	while (lexer.curr < lexer.end)
	{
		cmd = cmd_add (line);
		if (!cmd)
			return (FALSE);
		if (!cmd_parse (sh, &lexer, cmd))
		{
			eprint ("syntax error near unexpected token `%c'", *lexer.curr);
			return (FALSE);
		}
		ft_lexer_skip_char (&lexer, '|');
		line->count += 1;
	}
	return (TRUE);
}
