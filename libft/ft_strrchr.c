/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:12:24 by caburges          #+#    #+#             */
/*   Updated: 2024/11/11 12:07:34 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	start_index;

	start_index = ft_strlen(s);
	while (start_index >= 0)
	{
		if (s[start_index] == (unsigned char)c)
			return ((char *)&s[start_index]);
		start_index--;
	}
	return (NULL);
}
