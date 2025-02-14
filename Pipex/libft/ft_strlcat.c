/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 18:21:36 by caburges          #+#    #+#             */
/*   Updated: 2024/11/19 17:35:26 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	nul_found(char *dst, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		if (dst[i] == '\0')
			return (1);
		i++;
	}
	return (0);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dest_len;
	size_t	i;

	if (size == 0 || (!nul_found(dst, size)))
		return (ft_strlen(src) + size);
	dest_len = ft_strlen(dst);
	i = 0;
	while (src[i] && (i < size - dest_len - 1))
	{
		dst[dest_len + i] = src[i];
		i++;
	}
	dst[dest_len + i] = '\0';
	return (ft_strlen(src) + dest_len);
}
