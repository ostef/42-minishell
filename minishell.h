/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 15:48:25 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/18 17:27:35 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_err
{
	OK = 0,
	ERR_MEMORY = 1,
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
 * arg_count: The number of entries in `args`
 *
 * args_cap: The size of the `args` buffer in t_strs
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

typedef struct s_cmd
{
	t_str			flat_args;
	t_s64			flat_args_count;
	t_s64			flat_args_cap;
	t_str			*args;
	t_s64			args_count;
	t_s64			args_cap;
	t_redir			*first_redir;
	t_redir			*last_redir;
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
 * Returns the value of the variable, or NULL if it does not exist or the
 * variable has no value assigned to it.
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

t_cmd	*cmd_add(t_cmd_line *line);
t_bool	cmd_line_parse(t_shell *sh, t_cstr str, t_cmd_line *line);

/* Execution */

t_bool	cmd_is_builtin(t_cmd *cmd);
/*
 * Searches all directories in the PATH environment variable for a command
 * with the name `cmd_name` that can be executed, unless `cmd_name` is a path
 * (it starts with ~, . or /). `out_filename` receives the full path of the
 * command, allocated with ft_temp (), or `cmd_name` directly if it is a path.
 * Returns OK if a valid path was found, otherwise one of ERR_CMD_NO_SUCH_FILE,
 * ERR_CMD_PERM is returned if `cmd_name` is a path, or ERR_CMD_NOT_FOUND if
 * the command name is not a path.
 */
t_err	cmd_find_path(t_shell *sh, t_cstr cmd_name, t_cstr *out_filename);
/*
 * Executes the command `cmd`. If the command cannot be executed (cmd_find_path
 * failed), an error message is printed. This function is responsible for
 * opening pipes for cmd and cmd->next. The pipe is opened even if the command
 * cannot be executed, because it needs to be accessible by the next command.
 * The fork is always executed, and if an error occurs when executing the
 * command, exit is called with the appropriate exit code.
 */
void	cmd_exec(t_shell *sh, t_cmd *cmd);
t_int	cmd_exec_builtin(t_shell *sh, t_cmd *cmd);
/* Returns the exit code of the last command */
t_int	cmd_line_exec(t_shell *sh, t_cmd_line *line);

/* Builtins */

t_int	builtin_cd(t_shell *sh, t_cmd *cmd);
t_int	builtin_pwd(t_shell *sh, t_cmd *cmd);

#endif
