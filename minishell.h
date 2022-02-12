/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 16:51:25 by soumanso          #+#    #+#             */
/*   Updated: 2022/02/12 16:26:49 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

# define PROMPT_CHAR '$'

typedef struct s_cmd
{
	t_str	name_or_path;
	t_str	*args;
	t_s64	arg_count;
}	t_cmd;

typedef struct s_cmd_line
{
	t_str	str;
	t_cmd	*cmds;
	t_s64	cmd_count;
}	t_cmd_line;

void	free_cmd(t_cmd *cmd);
void	free_cmd_line(t_cmd_line *line);

t_bool	parse_cmd_line(t_cmd_line *line);
void	execute_cmd_line(t_cmd_line *line);

t_str	get_working_dir(void);

#endif
