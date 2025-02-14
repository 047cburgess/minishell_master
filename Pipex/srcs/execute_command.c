/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:48:18 by caburges          #+#    #+#             */
/*   Updated: 2025/01/28 16:44:46 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_paths(char **envp);
void	find_executable(t_pipe *data, char *cmd, char **envp);

void	execute_command(t_pipe *data, char *cmd, char **envp)
{
	data->args = ft_split(cmd, ' ');
	if (data->args == NULL)
		exit_malloc(data);
	data->paths = get_paths(envp);
	data->split_paths = ft_split(data->paths, ':');
	if (data->split_paths == NULL)
	{
		perror("malloc");
		clean_up_exit(data, EXIT_FAILURE);
	}
	find_executable(data, data->args[0], data->split_paths);
	execve(data->path, data->args, envp);
	clean_up_exit(data, EXIT_FAILURE);
}

int	test_raw_cmd(t_pipe *data, char *cmd)
{
	if (access(cmd, X_OK) == 0)
	{
		data->path = ft_strdup(cmd);
		if (!data->path)
		{
			perror("malloc");
			clean_up_exit(data, EXIT_FAILURE);
		}
		return (1);
	}
	return (0);
}

int	test_executable(t_pipe *data, char *executable, char *new_cmd)
{
	if (access(executable, X_OK) == 0)
	{
		ft_free(&new_cmd);
		data->path = executable;
		return (1);
	}
	return (0);
}

void	find_executable(t_pipe *data, char *cmd, char **paths)
{
	char	*new_cmd;
	char	*x;

	new_cmd = ft_strjoin("/", cmd);
	if (!new_cmd)
		exit_malloc(data);
	while (*paths != NULL)
	{
		x = ft_strjoin(*paths, new_cmd);
		if (!x)
		{
			ft_free(&new_cmd);
			exit_malloc(data);
		}
		if (test_executable(data, x, new_cmd))
			return ;
		ft_free(&x);
		paths++;
	}
	ft_free(&new_cmd);
	if (test_raw_cmd(data, cmd) && (ft_strchr(cmd, '/') != NULL))
		return ;
	ft_putstr_fd("Command not found\n", 2);
	clean_up_exit(data, EXIT_FAILURE);
}

char	*get_paths(char **envp)
{
	while (*envp != NULL)
	{
		if (ft_strncmp("PATH=", *envp, 5) == 0)
			return (*envp + 5);
		envp++;
	}
	return (NULL);
}
