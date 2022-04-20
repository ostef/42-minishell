/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandric <aandric@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 16:58:57 by soumanso          #+#    #+#             */
/*   Updated: 2022/04/20 16:48:32 by aandric          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_bool	eprint(t_cstr fmt_str, ...)
{
	va_list	va;

	va_start (va, fmt_str);
	ft_fprint (2, "minishell: ");
	ft_vfprintln (2, fmt_str, va);
	va_end (va);
	return (FALSE);
}
