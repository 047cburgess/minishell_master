/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 11:33:53 by caburges          #+#    #+#             */
/*   Updated: 2024/11/22 19:00:02 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	ft_printnbr(int fd, int nbr)
{
	char	*number;
	int		chars_printed;

	number = ft_itoa(nbr);
	if (number == NULL)
		return (0);
	chars_printed = ft_printstr(fd, number);
	free(number);
	return (chars_printed);
}
