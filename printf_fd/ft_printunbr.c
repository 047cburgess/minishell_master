/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printunbr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 11:37:59 by caburges          #+#    #+#             */
/*   Updated: 2025/02/27 10:56:41 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

static void	put_unbr(int fd, unsigned int nbr)
{
	if (nbr >= 10)
		put_unbr(fd, nbr / 10);
	ft_putchar_fd(nbr % 10 + 48, fd);
}

int	ft_printunbr(int fd, unsigned int nbr)
{
	int	num_len;

	put_unbr(fd, nbr);
	if (nbr == 0)
	{
		num_len = 1;
		return (num_len);
	}
	num_len = 0;
	while (nbr != 0)
	{
		nbr = nbr / 10;
		num_len++;
	}
	return (num_len);
}
