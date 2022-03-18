/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 16:10:35 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/18 17:56:50 by soumanso         ###   ########lyon.fr   */
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
		return (ft_strndup(str, len, ft_temp()));
	result = "";
	while (lexer.curr < lexer.end)
	{
		token = ft_lexer_skip_delim(&lexer, "$");
		result = ft_fmt(ft_temp(), "%s%.*s", result, token->len, token->str);
		ft_lexer_skip_char(&lexer, '$');
		token = ft_lexer_skip_char(&lexer, '?');
		if (token)
			result = (ft_fmt(ft_temp(), "%s%d", result, sh->last_exit_status));
		else
			token = ft_lexer_skip_identifier(&lexer);
		if (token)
		{	
			result = ft_fmt(ft_temp(), "%s%s", result, env_get(sh,
				ft_strndup(token->str, token->len, ft_temp())));
		}
		nb_dollars--;
	}
	return (result);
}

static t_redir	*cmd_add_redir(t_cmd *cmd, t_redir_kind kind, t_cstr filename, t_s64 len)
{
	t_redir	*new;
	t_redir	*tmp_next;

	new = (t_redir *)ft_zalloc (sizeof (t_redir), ft_temp ());
	if (!new)
		return (NULL);
	new->kind = kind;
	new->filename = ft_strndup (filename, len, ft_temp ());
	if (cmd->last_redir)
	{
		tmp_next = cmd->last_redir->next;
		cmd->last_redir->next = new;
		new->prev = cmd->last_redir;
		new->next = tmp_next;
		if (tmp_next)
			tmp_next->prev = new;
	}
	else
		cmd->first_redir = new;
	cmd->last_redir = new;
	return (new);
}

static t_redir_kind	cmd_parse_redir_symbol(t_lexer *lexer)
{
	t_token			*token;
	t_redir_kind	kind;

	token = ft_lexer_skip_string (lexer, "<<");
	kind = RD_IN_HERE;
	if (!token)
	{
		token = ft_lexer_skip_string (lexer, "<");
		kind = RD_IN;
	}
	if (!token)
	{
		token = ft_lexer_skip_string (lexer, ">>");
		kind = RD_OUT_APPEND;
	}
	if (!token)
	{
		token = ft_lexer_skip_string (lexer, ">");
		kind = RD_OUT;
	}
	if (!token)
		return (RD_NONE);
	ft_lexer_skip_spaces (lexer);
	return (kind);
}

static t_bool	cmd_parse(t_shell *sh, t_lexer *lexer, t_cmd *out)
{
	t_token			*token;
	t_redir_kind	redir_kind;

	while (lexer->curr < lexer->end)
	{
		ft_lexer_skip_spaces(lexer);
		redir_kind = cmd_parse_redir_symbol (lexer);
		token = ft_lexer_skip_quoted_str(lexer);
		if (!token)
			token = ft_lexer_skip_delim(lexer, "\v\t\n\r |");
		if (!token)
			break ;
		if (token->len > 0)
		{
			token->str = replace_dollars(sh, token->str, token->len);
			token->len = ft_strlen(token->str);
			if (redir_kind)
				cmd_add_redir (out, redir_kind, token->str, token->len);
			else
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
			sh->last_exit_status = 258;
			return (FALSE);
		}
		ft_lexer_skip_char (&lexer, '|');
		line->count += 1;
	}
	return (TRUE);
}
