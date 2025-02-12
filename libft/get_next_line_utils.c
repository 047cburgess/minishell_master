/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:18:24 by caburges          #+#    #+#             */
/*   Updated: 2025/01/12 18:47:09 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_join_buffers(char *vault, char *buffer)
{
	char	*joined_buffers;
	size_t	vault_len;

	if (!vault)
		return (NULL);
	vault_len = ft_strlen(vault);
	joined_buffers = malloc((vault_len + ft_strlen(buffer) + 1) * sizeof(char));
	if (!joined_buffers)
	{
		free(vault);
		return (NULL);
	}
	ft_strcpy(joined_buffers, vault);
	ft_strcpy(&joined_buffers[vault_len], buffer);
	free(vault);
	return (joined_buffers);
}
