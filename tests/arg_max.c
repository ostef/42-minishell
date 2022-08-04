/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_max.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 16:47:14 by soumanso          #+#    #+#             */
/*   Updated: 2022/08/04 16:47:14 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	main(int argc, char **args)
{
	int		i;
	t_s64	count;

	if (argc < 2)
	{
		ft_fprintln (STDERR, "Error: enter the number of arguments to use.");
		return (1);
	}
	if (ft_str_to_s64 (args[1], &count) != ft_strlen (args[1]))
	{
		ft_fprintln (STDERR, "Error: numeric value expected for argument 1.");
		return (1);
	}
	ft_print ("/bin/echo");
	i = 0;
	while (i < count)
	{
		ft_print (" '%i'", i);
		i += 1;
	}
	ft_print ("\n");
	return (0);
}
