/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soumanso <soumanso@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 16:58:57 by soumanso          #+#    #+#             */
/*   Updated: 2022/03/02 17:02:15 by soumanso         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	eprint(t_cstr fmt_str, ...)
{
	va_list	va;

	va_start (va, fmt_str);
	ft_fprint (2, "minishell: ");
	ft_vfprintln (2, fmt_str, va);
	va_end (va);
}
