/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:12:56 by caburges          #+#    #+#             */
/*   Updated: 2024/11/18 17:31:46 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	real_len(char const *s, unsigned int start, size_t len)
{
	size_t	i;

	i = 0;
	while (s[i + start] && i < len)
		i++;
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*new_string;
	size_t	sub_len;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s) || len == 0)
		return (ft_strdup(""));
	sub_len = real_len(s, start, len);
	new_string = malloc((sub_len + 1) * sizeof(char));
	if (!new_string)
		return (NULL);
	ft_strlcpy(new_string, &s[start], sub_len + 1);
	return (new_string);
}
