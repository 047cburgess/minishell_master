/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:57:36 by caburges          #+#    #+#             */
/*   Updated: 2025/03/12 10:57:39 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <sys/stat.h>

int	set_path_for_abs_or_rel_command(t_command *cmd, char *path, char *command)
{
	ft_strlcpy(path, command, FULL_PATH_MAX);
	if (access(path, F_OK != 0))
		cmd->error = 127;
	return (1);
}

// This function returns the full path for a given cmd (check file exists only)
int	set_cmd_path(t_data *data, char *path, char *command, t_command *cmd)
{
	int	i;

	i = 0;
	ft_bzero(path, FULL_PATH_MAX);
	if (cmd->error != 0 || is_builtin(cmd->av))
		return (0);
	if (ft_strchr(command, '/') || data->path_dirs == NULL)
		return (set_path_for_abs_or_rel_command(cmd, path, command));
	while (data->path_dirs[i] != NULL)
	{	
		ft_strlcpy(path, data->path_dirs[i], FULL_PATH_MAX);
		ft_strlcat(path, "/", FULL_PATH_MAX);
		ft_strlcat(path, command, FULL_PATH_MAX);
		if (access(path, F_OK) == 0)
			return (1);
		i++;
	}
	cmd->error = ER_CMD_NOT_FOUND;
	return (0);
}

// Checks if the command is a directory, and if it's executable
int	check_access(char *full_path, t_command *cmd)
{
	struct stat	status_buffer;

	if (cmd->error != 0 || is_builtin(cmd->av))
		return (0);
	stat(full_path, &status_buffer);
	if ((status_buffer.st_mode & S_IFMT) == S_IFDIR)
	{
		cmd->error = ER_IS_DIR;
		return (0);
	}
	if (access(full_path, X_OK) != 0)
	{
		cmd->error = 126;
		return (0);
	}
	return (SUCCESS);
}

// checks if the command is an empty string or no command provided at all
void	check_no_or_empty_command(t_command *cmd)
{
	if (cmd->ac == 0 && cmd->error == 0)
		cmd->error = ER_NO_CMD;
	else if (cmd->av[0][0] == '\0')
		cmd->error = ER_CMD_NOT_FOUND;
}
