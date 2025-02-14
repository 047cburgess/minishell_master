/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 14:38:48 by caburges          #+#    #+#             */
/*   Updated: 2025/01/28 16:35:22 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_split(char **array);
void	close_pipe(t_pipe *data);

void	clean_up_exit(t_pipe *data, int status)
{
	close_pipe(data);
	free_split(data->args);
	data->args = NULL;
	free_split(data->split_paths);
	data->split_paths = NULL;
	free(data->path);
	exit(status);
}

void	free_split(char **array)
{
	int	i;

	i = 0;
	if (array != NULL)
	{
		while (array[i] != NULL)
		{
			ft_free(&array[i]);
			i++;
		}
		free(array);
	}
}

void	close_pipe(t_pipe *data)
{
	if (data->pipe_fd[0] >= 0)
		close(data->pipe_fd[0]);
	if (data->pipe_fd[1] >= 0)
		close(data->pipe_fd[1]);
}

void	exit_malloc(t_pipe *data)
{
	perror("malloc");
	clean_up_exit(data, EXIT_FAILURE);
}
