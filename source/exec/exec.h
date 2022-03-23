/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 15:31:09 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/23 19:01:16 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

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
 * For non-builtins, the fork is always executed, and if an error occurs when
 * executing the command, exit is called with the appropriate exit code.
 * For builtins, a function is called directly in the shell process, without
 * forking.
 */
void	cmd_exec(t_shell *sh, t_cmd *cmd);
t_int	cmd_exec_builtin(t_shell *sh, t_cmd *cmd);

t_bool	ft_redir(t_shell *shell, t_cmd *cmd);
t_bool	ft_redir_out(t_shell *shell, t_cmd *cmd);
t_bool	ft_redir_in(t_shell *shell, t_cmd *cmd);

#endif
