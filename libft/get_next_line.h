/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 10:48:04 by caburges          #+#    #+#             */
/*   Updated: 2025/01/10 16:40:47 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 150
# endif

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

char	*get_next_line(int fd);
int		ft_linelen(char *s);
char	*ft_extract_line(char *vault);
void	ft_set_new_start(char **vault);
char	*ft_fill_vault(int fd, char *vault);

// Utils
size_t	my_strlen(char *str);
char	*ft_join_buffers(char *vault, char *buffer);
char	*my_strchr(char *s, int c);
char	*my_strcpy(char *dest, char *src);
char	*my_calloc(size_t nmemb, size_t size);

#endif
