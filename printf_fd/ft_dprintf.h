/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 14:09:55 by caburges          #+#    #+#             */
/*   Updated: 2025/02/27 11:08:19 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DPRINTF_H
# define FT_DPRINTF_H

# include "libft.h"

typedef struct s_printf
{
	t_list	*cutting;
	int		error;
	char	*final_line;
	int		line_len;

}	t_printf;

int		ft_dprintf(int fd, const char *str, ...);
char	*ft_printstr(char *s);
char	*ft_printnbr(int nbr);
void	init_printf(t_printf *data);

#endif
