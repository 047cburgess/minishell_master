/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 10:54:12 by caburges          #+#    #+#             */
/*   Updated: 2025/01/12 18:57:53 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// PURPOSE: Calculate length of the full line -> either by nl or \0 if EOF
static int	ft_linelen(char *s)
{
	int	line_len;

	line_len = 0;
	while (s[line_len] != '\n' && s[line_len] != '\0')
		line_len++;
	if (s[line_len] == '\n')
		line_len++;
	return (line_len);
}

// PURPOSE: Extract just the whole line to be return 
static char	*ft_extract_line(char *vault)
{
	char	*new_line;
	int		line_len;
	int		i;

	if (!vault)
		return (NULL);
	line_len = ft_linelen(vault);
	new_line = ft_calloc(line_len + 1, sizeof(char));
	if (!new_line)
		return (NULL);
	i = 0;
	while (i < line_len)
	{
		new_line[i] = vault[i];
		i++;
	}
	return (new_line);
}

// PURPOSE: Reset the line to just the remaining characters for next read
static void	ft_set_new_start(char **vault)
{
	char	*new_line_char_found;

	if (*vault != NULL)
	{
		new_line_char_found = ft_strchr(*vault, '\n');
		if (new_line_char_found)
			ft_strcpy(*vault, new_line_char_found + 1);
		else
			(*vault)[0] = '\0';
	}
}

// PURPOSE: Read into buffer and build until new line or EOF is found
static char	*ft_fill_vault(int fd, char *vault)
{
	char	*buffer;
	int		bytes_read;

	buffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!buffer)
		return (free(vault), NULL);
	bytes_read = 1;
	while (ft_strchr(vault, '\n') == NULL && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1 || (bytes_read == 0 && vault[0] == '\0'))
		{
			free(buffer);
			return (free(vault), NULL);
		}
		buffer[bytes_read] = '\0';
		vault = ft_join_buffers(vault, buffer);
		if (!vault)
			return (free(buffer), NULL);
	}
	free(buffer);
	return (vault);
}

// PURPOSE: extracts complete line from a file, one line at a time
char	*get_next_line(int fd)
{
	static char	*vault;
	char		*new_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free(vault), NULL);
	if (vault == NULL)
	{
		vault = ft_calloc(1, sizeof(char));
		if (!vault)
			return (NULL);
	}
	if (ft_strchr(vault, '\n') == NULL)
		vault = ft_fill_vault(fd, vault);
	new_line = ft_extract_line(vault);
	if (!new_line)
	{
		free(vault);
		vault = NULL;
		return (NULL);
	}
	ft_set_new_start(&vault);
	return (new_line);
}
