/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 17:17:40 by aandric           #+#    #+#             */
/*   Updated: 2022/03/18 19:23:04 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_bool	ft_redir(t_shell *shell, t_cmd *cmd)
{
	if (!cmd->first_redir)
		return (0);
	if (kind == RD_NONE)
	return (1);
}

t_bool ft_redir_out(t_shell *shell, t_cmd *cmd)
{
	int	fd_in;
	int	fd_out;

	fd_in = open(cmd->first_redir->filename, O_RDONLY);
	if (fd_in < 0)
		return (0);
	fd_out = open(cmd->last_redir->filename, O_WRONLY);
	if (fd_out < 0)
		return (0);
	dup2(fd_in, fd_out);
}