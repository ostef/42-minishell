/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 15:48:25 by soumanso          #+#    #+#             */
/*   Updated: 2022/02/25 16:01:28 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <unistd.h>

typedef struct s_env
{
	t_str			name;
	t_str			val;
	struct s_env	*prev;
	struct s_env	*next;
}	t_env;

# define PIPE_READ 0
# define PIPE_WRITE 1

typedef struct s_cmd
{
	t_str			in_filename;
	t_str			out_filename;
	t_bool			out_append;	/* >> */
	t_str			name;
	t_str			*args;
	t_s64			arg_count;
	/* Execution info, this gets used and set when calling cmd_exec */
	pid_t			pid;
	t_file			pipe[2];
	struct s_cmd	*prev;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_cmd_line
{
	t_cmd	*first;
	t_cmd	*last;
	t_int	count;
}	t_cmd_line;

typedef struct s_shell
{
	t_str	*env_original;
	t_env	*env;
}	t_shell;

/* Environment */

t_bool	env_parse(t_cstr str, t_env *out);
t_cstr	env_get(t_shell *sh, t_cstr name);
void	env_set(t_shell *sh, t_cstr name, t_cstr val);
void	env_remove(t_shell *sh, t_cstr name);

/* Command line parsing */

t_bool	cmd_line_parse(t_cstr str, t_cmd_line *out);

/* Execution */

t_cmd	*cmd_add(t_cmd_line *line);
void	cmd_exec(t_shell *sh, t_cmd *cmd);
/* Returns the exit code of the last command */
t_int	cmd_line_exec(t_shell *sh, t_cmd_line *line);

#endif
