/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:51:55 by caburges          #+#    #+#             */
/*   Updated: 2024/11/19 18:00:04 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	word_count;

	word_count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s == '\0')
			return (word_count);
		word_count++;
		while (*s && *s != c)
			s++;
	}
	return (word_count);
}

static char	**free_array(char **array, size_t words_to_free)
{
	size_t	i;

	i = 0;
	while (i < words_to_free)
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (NULL);
}

static size_t	count_word_len(char const *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	size_t	i;
	size_t	word_len;

	if (!s)
		return (NULL);
	array = ft_calloc((count_words(s, c) + 1), sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	while (*s != '\0')
	{
		while (*s == c)
			s++;
		if (*s != '\0')
		{
			word_len = count_word_len(s, c);
			array[i] = ft_substr(s, 0, word_len);
			if (!array[i])
				return (free_array(array, i));
			i++;
			s = s + word_len;
		}
	}
	return (array);
}
