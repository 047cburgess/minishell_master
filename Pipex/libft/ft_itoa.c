/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:43:50 by caburges          #+#    #+#             */
/*   Updated: 2024/11/13 18:24:45 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	calculate_number_len(int n)
{
	int				len;
	unsigned int	u_n;

	len = 0;
	if (n < 0)
	{
		len++;
		u_n = -n;
	}
	else
		u_n = n;
	while (u_n > 0)
	{
		u_n = u_n / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char				*result;
	int					number_len;
	unsigned int		u_n;

	if (n == 0)
		return (ft_strdup("0"));
	number_len = calculate_number_len(n);
	result = ft_calloc((number_len + 1), sizeof(char));
	if (!result)
		return (NULL);
	if (n < 0)
	{
		u_n = -n;
		result[0] = '-';
	}
	else
		u_n = n;
	while (u_n > 0)
	{
		result[number_len - 1] = u_n % 10 + 48;
		u_n = u_n / 10;
		number_len--;
	}
	return (result);
}
