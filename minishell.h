/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 15:48:25 by soumanso          #+#    #+#             */
/*   Updated: 2022/04/01 18:00:45 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

# define DEFAULT_PATH "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
# define PIPE_READ 0
# define PIPE_WRITE 1

typedef enum e_err
{
	OK = 0,
	ERR_MEMORY = 1,
	ERR_CMD_IS_DIR = 126,
	ERR_CMD_NO_SUCH_FILE = 127,
	ERR_CMD_PERM = 128,
	ERR_CMD_NOT_FOUND = 129,
}	t_err;

typedef struct s_env
{
	t_str			name;
	t_str			val;
	struct s_env	*prev;
	struct s_env	*next;
}	t_env;

/*
 * flat_args: All the command arguments in a flat array, separated by \0s.
 * This includes the name of the command.
 *
 * flag_args_count: The number of arguments following each other in `flag_args`
 * 
 * flat_args_cap: The size of the `flat_args` buffer in chars
 *
 * args: All the command arguments as an array of strings, indexing in
 * `flat_args`. This includes the name of the command and is null-terminated
 *
 * arg_count: The number of entries in `args`
 *
 * args_cap: The size of the `args` buffer in t_strs
 *
 * redir_first: The first node of the linked list of redirections
 * 
 * redir_last: The last node of the linked list of redirections
 *
 * pid: The PID of the command when it gets executed.
 *
 * pipe: An array of 2 file descriptors containing the files for inter-process
 * communication.
 * The pipe is used for communication between commands N and N + 1,
 * pipe[PIPE_READ] is the fd to read from, pipe[PIPE_WRITE] is the fd to
 * write to. If there is no pipe, then both are set to 0.
 *
 * builtin_exit_status: The exit status of the command when it finished,
 * if it is a builtin.
 */

typedef enum e_redir_kind
{
	RD_NONE = 0,
	RD_OUT = 1,
	RD_OUT_APPEND = 2,
	RD_IN = 3,
	RD_IN_HERE = 4
}	t_redir_kind;

typedef struct s_redir
{
	t_redir_kind	kind;
	t_str			filename;
	struct s_redir	*prev;
	struct s_redir	*next;
	
}	t_redir;

typedef struct s_heredoc
{
	t_str				here_line;
	struct s_heredoc	*next;
}	t_heredoc;

typedef struct s_cmd
{
	t_str			flat_args;
	t_s64			flat_args_count;
	t_s64			flat_args_cap;
	t_str			*args;
	t_s64			args_count;
	t_s64			args_cap;
	t_redir			*redir_first;
	t_redir			*redir_link;
	t_redir			*redir_last;
	t_heredoc		*heredoc_link;
	t_file			fd_in;
	t_file			fd_out;
	t_bool			has_errors;
	pid_t			pid;
	t_file			pipe[2];
	t_int			builtin_exit_status;
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
	t_env	*env_first;
	t_env	*env_last;
	t_int	env_count;
	t_int	last_exit_status;
	t_bool	should_exit;
}	t_shell;

/* Error */

void	eprint(t_cstr fmt_str, ...);

/* Environment variables */

/*
 * Parse one environment variable line, in the form A=B
 * Puts the result in `env`
 * Returns TRUE on success, FALSE on failure.
 * 
 * Valid variable names consist of letters, underscores and digits, and do not
 * begin with a digit.
 */
t_bool	env_parse(t_cstr str, t_env *env);
/*
 * Retrieve the environment variable named `name` inside the sh->env list.
 * Returns the list node of the variable, or NULL if it does not exist.
 */
t_env	*env_get_node(t_shell *sh, t_cstr name);
/*
 * Retrieve the environment variable named `name` inside the sh->env list.
 * Returns the value of the variable, or "" if the variable does not exist.
 */
t_cstr	env_get(t_shell *sh, t_cstr name);
/*
 * Set the value of the environment variable named `name` inside the
 * sh->env list. If the variable does not exist, adds it to the list.
 * Returns TRUE if the variable was in the list, FALSE otherwise.
 * If val is NULL, then the variable won't be overwritten if it already
 * exists (see the behaviour of `export`).
 */
t_bool	env_set(t_shell *sh, t_cstr name, t_cstr val);
/*
 * Removes the environment variable named `name` from the sh->env list.
 * Returns TRUE if the variable was in the list, FALSE otherwise.
 */
t_bool	env_remove(t_shell *sh, t_cstr name);

/* Command line parsing */

t_bool	cmd_line_parse(t_shell *sh, t_cstr str, t_cmd_line *line);

/* Execution */

/* Returns the exit code of the last command */
t_int	cmd_line_exec(t_shell *sh, t_cmd_line *line);

/* Builtins */

t_int	builtin_echo(t_shell *sh, t_cmd *cmd);
t_int	builtin_cd(t_shell *sh, t_cmd *cmd);
t_int	builtin_pwd(t_shell *sh, t_cmd *cmd);
t_int	builtin_exit(t_shell *sh, t_cmd *cmd);

/* Signals */

void	int_handler(int signo);
void	sig_handler(void);
void	sig_nl(void);
void	silence(int signo);
void	put_nl(int signo);
void	quit_3(int signo);

void	rl_replace_line(const char *text, int clearundo);

#endif
