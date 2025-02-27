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

int	ft_dprintf(int fd, const char *str, ...);
int	ft_printchar(int fd, const char c);
int	ft_printstr(int fd, char *s);
int	ft_printnbr(int fd, int nbr);
int	ft_printunbr(int fd, unsigned int nbr);
int	ft_printaddress(int fd, void *address);
int	ft_printhex(int fd, unsigned long nbr, char format);

#endif
