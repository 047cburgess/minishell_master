/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:45:56 by caburges          #+#    #+#             */
/*   Updated: 2024/11/15 12:22:20 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	u_n;

	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		u_n = -n;
	}
	else
		u_n = n;
	if (u_n >= 10)
		ft_putnbr_fd(u_n / 10, fd);
	ft_putchar_fd((u_n % 10) + 48, fd);
}
