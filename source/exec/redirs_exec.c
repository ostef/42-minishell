/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 17:17:40 by aandric           #+#    #+#             */
/*   Updated: 2022/03/24 14:28:25 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

t_bool	ft_redir_out(t_shell *shell, t_cmd *cmd)
{
	int		fd_out;
	t_redir	*temp;

	temp = cmd->redir_link;
	while (temp)
	{
		fd_out = open(temp->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd_out < 0)
		{
			write(2, "outfile not found\n", 18);
			return (0);
		}
		if (temp->kind == RD_OUT)
		{
			dup2(fd_out, STDOUT);
			close(fd_out);
		}
		temp = temp->next;
	}
	return (1);
}

t_bool	ft_redir_out_append(t_shell *shell, t_cmd *cmd)
{
	int		fd_out;
	t_redir	*temp;

	temp = cmd->redir_link;
	while (temp)
	{
		fd_out = open(temp->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (fd_out < 0)
		{
			write(2, "outfile not found\n", 18);
			return (0);
		}
		if (temp->kind == RD_OUT_APPEND)
		{
			dup2(fd_out, STDOUT);
			close(fd_out);
		}
		temp = temp->next;
	}
	return (1);
}

t_bool	ft_redir_in(t_shell *shell, t_cmd *cmd)
{
	int		fd_in;
	t_redir	*temp;

	temp = cmd->redir_link;
	while (temp)
	{
		if (temp->kind == RD_IN)
		{
			fd_in = open(temp->filename, O_RDONLY);
			if (fd_in < 0)
			{
				write(2, "infile not found\n", 17);
				return (0);
			}
			dup2(fd_in, STDIN);
			close(fd_in);
		}
		temp = temp->next;
	}
	return (1);
}
/*
t_bool	ft_redir_here(t_shell *shell, t_cmd *cmd)
{
	int		fd_in;
	t_redir	*temp;

	temp = cmd->redir_first;
	while (temp)
	{
		if (temp->kind == RD_IN_HERE)
		{
			//fd_in = open(temp->filename, O_WRONLY | O_RDONLY);
			//if (fd_in < 0)
			//	return (0);
			dup2(fd_in, STDOUT);
			close(fd_in);
		}
		temp = temp->next;
	}
	return (1);
}
*/

t_bool	ft_redir(t_shell *shell, t_cmd *cmd)
{
	cmd->redir_link = cmd->redir_first;
	
	while (cmd->redir_link)
	{
		if (!cmd->redir_link)
			return (0);
		if (cmd->redir_link->kind == RD_OUT)
			ft_redir_out(shell, cmd);
		if (cmd->redir_link->kind == RD_IN)
			ft_redir_in(shell, cmd);
		if (cmd->redir_link->kind == RD_OUT_APPEND)
			ft_redir_out_append(shell, cmd);
		/*if (cmd->redir_first->kind == RD_IN_HERE)
			ft_redir_here(shell, cmd);*/
		cmd->redir_link = cmd->redir_link->next;
	}
	return (1);
}
