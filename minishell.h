/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 15:48:25 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/09 17:24:34 by aandric          ###   ########lyon.fr   */
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

/*
 * in_filename, out_filename: Input and output redirection filenames, set to
 * NULL if there is no redirection
 *
 * out_append: This is set to TRUE if the output redirection is of type >>
 *
 * flat_args: All the command arguments in a flat array, separated by \0s.
 * This includes the name of the command.
 * 
 * flat_args_cap: The size of the `flat_args` buffer in chars
 *
 * args: All the command arguments as an array of strings, indexing in
 * `flat_args`. This includes the name of the command and is null-terminated
 * 
 * args_cap: The size of the `args` buffer in t_strs
 *
 * arg_count: The number of entries in `args`
 *
 * pid: The PID of the command when it gets executed.
 *
 * pipe: An array of 2 file descriptors containing the files for inter-process
 * communication.
 * The pipe is used for communication between commands N and N + 1,
 * pipe[PIPE_READ] is the fd to read from, pipe[PIPE_WRITE] is the fd to
 * write to. If there is no pipe, then both are set to 0.
 */

typedef struct s_cmd
{
	t_str			in_filename;
	t_str			out_filename;
	t_bool			out_append;
	t_str			flat_args;
	t_s64			flat_args_count;
	t_s64			flat_args_cap;
	t_str			*args;
	t_s64			args_cap;
	t_s64			args_count;
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
	t_env	*env_first;
	t_env	*env_last;
	t_int	last_exit_code;
}	t_shell;

/* Error */

void	eprint(t_cstr fmt_str, ...);

/* Environment variables */

/*
 * Parse one environment variable line, in the form A=B
 * Puts the result in `env`
 * Returns TRUE on success, FALSE on failure (the name of the variable is invalid)
 * 
 * Valid variable names consist of letters, underscores and digits, and do not
 * begin with a digit.
 */
t_bool	env_parse(t_cstr str, t_env *env);
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

void	cmd_exec(t_shell *sh, t_cmd *cmd);
/* Returns the exit code of the last command */
t_int	cmd_line_exec(t_shell *sh, t_cmd_line *line);

#endif
