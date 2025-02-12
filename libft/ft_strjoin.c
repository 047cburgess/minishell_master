/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:33:38 by caburges          #+#    #+#             */
/*   Updated: 2024/11/12 16:29:52 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new_string;
	size_t	buffer_size;

	if (!s1 || !s2)
		return (NULL);
	buffer_size = ft_strlen(s1) + ft_strlen(s2) + 1;
	new_string = malloc(buffer_size * sizeof(char));
	if (!new_string)
		return (NULL);
	ft_strlcpy(new_string, s1, buffer_size);
	ft_strlcat(new_string, s2, buffer_size);
	return (new_string);
}
