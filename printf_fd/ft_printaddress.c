/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printaddress.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:59:26 by caburges          #+#    #+#             */
/*   Updated: 2024/11/25 15:24:39 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	ft_printaddress(int fd, void *address)
{
	int					chars_printed;
	unsigned long		casted_address;

	if (address == NULL)
	{
		chars_printed = ft_printstr(fd, "(nil)");
		return (chars_printed);
	}
	casted_address = (unsigned long)address;
	chars_printed = ft_printstr(fd, "0x");
	chars_printed += ft_printhex(fd, casted_address, 'x');
	return (chars_printed);
}
