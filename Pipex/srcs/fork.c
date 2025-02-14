/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:14:58 by caburges          #+#    #+#             */
/*   Updated: 2025/01/27 16:43:30 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	run_command_1(t_pipe *pipe, char **av, char **envp)
{
	close(pipe->pipe_fd[0]);
	pipe->in_fd = open(av[1], O_RDONLY);
	if (pipe->in_fd < 0)
	{
		close(pipe->pipe_fd[1]);
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(pipe->in_fd, STDIN_FILENO);
	close(pipe->in_fd);
	dup2(pipe->pipe_fd[1], STDOUT_FILENO);
	close(pipe->pipe_fd[1]);
	execute_command(pipe, av[2], envp);
}

void	run_command_2(t_pipe *pipe, char **av, char **envp)
{
	close(pipe->pipe_fd[1]);
	pipe->out_fd = open(av[4], O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (pipe->out_fd < 0)
	{
		close(pipe->pipe_fd[1]);
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(pipe->out_fd, STDOUT_FILENO);
	close(pipe->out_fd);
	dup2(pipe->pipe_fd[0], STDIN_FILENO);
	close(pipe->pipe_fd[0]);
	execute_command(pipe, av[3], envp);
}
