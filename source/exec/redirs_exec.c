/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 17:17:40 by aandric           #+#    #+#             */
/*   Updated: 2022/03/23 19:21:32 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

t_bool ft_redir_out(t_shell *shell, t_cmd *cmd)
{
	int		fd_out;
	t_redir	*temp;
	
	temp = cmd->redir_first;
	while (temp)
	{
		fd_out = open(temp->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd_out < 0)
			return (0);
		if (temp->kind == RD_OUT)
		{
			dup2(fd_out, STDOUT);
			close(fd_out);
		}
		temp = temp->next;
	}
	//close(fd_out);
	return (1);
}

t_bool ft_redir_in(t_shell *shell, t_cmd *cmd)
{
	int		fd_in;
	t_redir	*temp;
	
	temp = cmd->redir_first;
	while (temp)
	{
		
		if (temp->kind == RD_IN)
		{
			fd_in = open(temp->filename, O_RDONLY);
			if (fd_in < 0)
				return (0);
			dup2(fd_in, STDIN);
			close(fd_in);
		}
		temp = temp->next;
	}
	return (1);
}

t_bool ft_redir_here(t_shell *shell, t_cmd *cmd)
{
	int		fd_in;
	t_redir	*temp;
	
	temp = cmd->redir_first;
	while (temp)
	{
		if (temp->kind == RD_IN_HERE)
		{
			fd_in = open(temp->filename, O_WRONLY | O_RDONLY);
			if (fd_in < 0)
				return (0);
			dup2(fd_in, STDOUT);
			close(fd_in);
		}
		temp = temp->next;
	}
	return (1);
}

t_bool	ft_redir(t_shell *shell, t_cmd *cmd)
{
	if (!cmd->redir_first)
		return (0);
	if (cmd->redir_first->kind == RD_OUT)
		ft_redir_out(shell, cmd);
	if (cmd->redir_first->kind == RD_IN)
		ft_redir_in(shell, cmd);
	/*if (cmd->redir_first->kind == RD_OUT_APPEND)
		ft_redir_append(shell, cmd);
	if (cmd->redir_first->kind == RD_IN_HERE)
		ft_redir_here(shell, cmd);*/
	return (1);
}

