/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printhex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:29:04 by caburges          #+#    #+#             */
/*   Updated: 2024/11/25 16:59:14 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

static void	puthex(int fd, unsigned long nbr, char *base)
{
	if (nbr >= 16)
		puthex(fd, nbr / 16, base);
	ft_printchar(fd, base[nbr % 16]);
}

int	ft_printhex(int fd, unsigned long nbr, char format)
{
	int	chars_printed;

	if (format == 'X')
		puthex(fd, nbr, "0123456789ABCDEF");
	else
		puthex(fd, nbr, "0123456789abcdef");
	if (nbr == 0)
		return (1);
	chars_printed = 0;
	while (nbr != 0)
	{
		nbr = nbr / 16;
		chars_printed++;
	}
	return (chars_printed);
}
