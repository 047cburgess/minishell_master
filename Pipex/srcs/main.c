/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 20:28:11 by caburges          #+#    #+#             */
/*   Updated: 2025/01/28 17:20:07 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int ac, char **av, char **envp)
{
	t_pipe	pipe;

	if ((!check_ac(ac)) || (!check_envp(envp)))
		return (1);
	init_struct(&pipe);
	set_up_pipe(pipe.pipe_fd);
	pipe.pid1 = fork();
	if (pipe.pid1 == -1)
		perror("fork 1");
	if (pipe.pid1 == 0)
		run_command_1(&pipe, av, envp);
	pipe.pid2 = fork();
	if (pipe.pid2 == -1)
		perror("fork 2");
	if (pipe.pid2 == 0)
		run_command_2(&pipe, av, envp);
	close_pipe(&pipe);
	if (pipe.pid1 != -1)
		waitpid(pipe.pid1, NULL, 0);
	if (pipe.pid2 != -1)
		waitpid(pipe.pid2, NULL, 0);
}
