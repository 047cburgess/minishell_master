/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printstr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 11:27:52 by caburges          #+#    #+#             */
/*   Updated: 2025/02/27 10:56:08 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	ft_printstr(int fd, char *s)
{
	if (s == NULL)
		return (write(fd, "(null)", 6));
	ft_putstr_fd(s, fd);
	return (ft_strlen(s));
}
