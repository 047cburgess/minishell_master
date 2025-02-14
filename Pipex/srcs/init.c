/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 14:24:40 by caburges          #+#    #+#             */
/*   Updated: 2025/01/27 14:57:40 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_struct(t_pipe *data)
{
	data->args = NULL;
	data->paths = NULL;
	data->split_paths = NULL;
	data->path = NULL;
}

int	check_ac(int ac)
{
	if (ac != 5)
	{
		ft_putendl_fd("Usage: infile \"cmd1\" \"cmd2\" outfile", 2);
		return (0);
	}
	return (1);
}

int	check_envp(char **envp)
{
	if (*envp == NULL)
	{
		ft_putendl_fd("Error: no envp set", 2);
		return (0);
	}
	return (1);
}
