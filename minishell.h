/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 15:48:25 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/02 16:19:17 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

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
	t_str			*args;	/* Null terminated, includes name */
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
	t_str	*env_original;	/* @Cleanup: remove this when we have env parsing and stuff */
	t_env	*env;
	t_int	last_exit_code;
}	t_shell;

/* Environment variables */

/*
 * Parse one environment variable line, in the form A=B
 * Puts the result in `out`
 * Returns TRUE on success, FALSE on failure (the name of the variable is invalid)
 * 
 * Valid variable names consist of letters, underscores and digits, and do not
 * begin with a digit.
 */
t_bool	env_parse(t_cstr str, t_env *out);
/*
 * Retrieve the environment variable named `name` inside the sh->env list.
 * Returns the value of the variable, or NULL if it does not exist.
 */
t_cstr	env_get(t_shell *sh, t_cstr name);
/*
 * Set the value of the environment variable named `name` inside the
 * sh->env list. If the variable does not exist, adds it to the list.
 * Returns TRUE if the variable was in the list, FALSE otherwise.
 */
t_bool	env_set(t_shell *sh, t_cstr name, t_cstr val);
/*
 * Removes the environment variable named `name` from the sh->env list.
 * Returns TRUE if the variable was in the list, FALSE otherwise.
 */
t_bool	env_remove(t_shell *sh, t_cstr name);

/* Command line parsing */

t_cmd	*cmd_add(t_cmd_line *line);
t_bool	cmd_line_parse(t_cstr str, t_cmd_line *line);

/* Execution */

t_str	*cmd_get_argv (t_cmd *cmd);
void	cmd_exec(t_shell *sh, t_cmd *cmd);
/* Returns the exit code of the last command */
t_int	cmd_line_exec(t_shell *sh, t_cmd_line *line);

#endif
