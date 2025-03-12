/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_printf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:59:37 by caburges          #+#    #+#             */
/*   Updated: 2025/03/12 10:59:38 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

void	init_printf(t_printf *data)
{
	data->cutting = NULL;
	data->error = 0;
	data->final_line = NULL;
	data->line_len = -1;
}
