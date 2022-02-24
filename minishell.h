/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 15:48:25 by soumanso          #+#    #+#             */
/*   Updated: 2022/02/24 18:18:11 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_env
{
	t_str	name;
	t_str	val;
}	t_env;

typedef struct s_cmd
{
	t_str	in_filename;
	t_str	out_filename;
	t_bool	out_append;	/* >> */
	t_str	name;
	t_str	*args;
}	t_cmd;

typedef struct s_cmd_line
{
	t_cmd	*cmds;
	t_int	cmd_count;
}	t_cmd_line;

typedef struct s_shell
{
	t_env	*env;
	
}	t_shell;

/* Environment */

t_bool	env_parse(t_str str, t_env *out);
t_str	env_get(t_shell *sh, t_str name);
void	env_set(t_shell *sh, t_str name, t_str val);
void	env_remove(t_shell *sh, t_str name);

/* Command line parsing */



/* Execution */

/* Returns the exit code of the last command */
t_int	cmd_line_exec(t_shell *sh, t_cmd_line *line);

#endif
