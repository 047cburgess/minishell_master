/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:38:47 by caburges          #+#    #+#             */
/*   Updated: 2025/02/24 14:54:06 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// NEED TO STORE
// 	pids for each process (or built in) to get exit status
//
// solo builtin
//
// solo non-builtin
//
// pipeline
// 	1st in pipe
// 	middle in pipe
// 	last in pipe
//
// T_CMD
// int	pipe_fds[2];
// int	in_out_fds[2];
// char **av;
// int	pid;
// int  is_heredoc
//

int	launch_solo_command(t_data *data)
{
	char	*command;
	int	in_out[2];
	pid_t	pid;

	command = get_command(data->tokens_list);
	if (is_builtin(command))
	{
		//Execute it and return the status??
	}
	else
	{
		pid = fork();
		if (pid = -1)
			return (FAILURE);
		if (pid == 0)
		{
			
		}
		
		
	}

}
