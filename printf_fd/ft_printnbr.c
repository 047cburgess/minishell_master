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

char	*ft_printnbr(int nbr)
{
	char	*number;

	number = ft_itoa(nbr);
	if (!number)
		return (NULL);
	else
		return (number);
}
