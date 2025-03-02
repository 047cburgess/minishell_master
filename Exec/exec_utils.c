#include "minishell.h"
#include <sys/wait.h>
#include <sys/stat.h>

// This function returns the full path for a given command, ready to give to execve
// If it's not found, it returns NULL

int	set_command_path(t_data *data, char *path, char *command)
{
	int	i;

	dprintf(data->log, "FUNCTION: Entered get_command_path\n");
	// Checks if it's a relative or absolute path already given
	if (ft_strchr(command, '/') || data->path_dirs == NULL)
	{
		dprintf(data->log, "No '\\' in command name or path is unset\n");
		ft_strlcpy(path, command, FULL_PATH_MAX);
		return (1);
	}

	// Checks all path folders to see if it's correct
	i = 0;
	while (data->path_dirs[i] != NULL)
	{	
		ft_strlcpy(path, data->path_dirs[i], FULL_PATH_MAX);
		ft_strlcat(path, "/", FULL_PATH_MAX);
		ft_strlcat(path, command, FULL_PATH_MAX);
		dprintf(data->log, "Checking path: %s\n", path);
		if (access(path, F_OK) == 0)
		{
			dprintf(data->log, "Executable found: %s\n", path);
			return (1);
		}
		i++;
	}
	ft_dprintf(2, "%s: command not found\n", command);
	ft_dprintf(data->log, "%s: command not found\n", command);
	clean_up_exit(data, 127, NULL);
	return (0);
}

int	check_access(char *full_path, t_data *data, t_command *cmd)
{
	struct stat	status_buffer;
	
	// Check it's executable
	if (access(full_path, X_OK) != 0)
	{
		ft_dprintf(2, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		ft_dprintf(data->log, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		close_fds(cmd);
		clean_up_exit(data, 126, NULL);
	}
	dprintf(data->log, "%s is executable\n", cmd->av[0]);

	// Check it's not a directory
	stat(full_path, &status_buffer);
	if ((status_buffer.st_mode & S_IFMT) == S_IFDIR)
	{
		ft_dprintf(2, "minishell: %s: Is a directory\n", cmd->av[0]);
		ft_dprintf(data->log, "minishell: %s: Is a directory\n", cmd->av[0]);
		close_fds(cmd);
		clean_up_exit(data, 126, NULL);
	}
	dprintf(data->log, "%s is not a directory\n", cmd->av[0]);
	return (SUCCESS);
}

void	close_fds(t_command *cmd)
{
	if (cmd->fds[0] > STDIN_FILENO)
		close(cmd->fds[0]);
	if (cmd->fds[1] > STDOUT_FILENO)
		close(cmd->fds[1]);
}
