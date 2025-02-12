/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 17:09:56 by caburges          #+#    #+#             */
/*   Updated: 2024/11/15 12:15:37 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*new_memory;

	new_memory = NULL;
	if (nmemb > 0 && ((size_t) - 1 / nmemb < size))
		return (NULL);
	new_memory = malloc(nmemb * size);
	if (!new_memory)
		return (NULL);
	ft_bzero(new_memory, nmemb * size);
	return (new_memory);
}
