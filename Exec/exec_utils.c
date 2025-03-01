#include "minishell.h"
#include <sys/wait.h>
#include <sys/stat.h>

// This function returns the full path for a given command, ready to give to execve
// If it's not found, it returns NULL


//TODO: BREAK THIS DOWN INTO SUBFUNCTIONS 
//TODO: DECIDE ON THE CONTROL FLOW TO MANAGE EXITS

char	*get_command_path(t_data *data, char *command)
{
	char	*full_path;
	char	**dirs;
	int	directory_length;
	int	command_length;
	int	full_path_size;
	int	i;

	dprintf(data->log, "FUNCTION: Entered get_command_path\n");
	// Checks if it's a relative or absolute path already given
	if (ft_strchr(command, '/'))
	{
		dprintf(data->log, "No '\\' in command name \n");
		full_path = ft_strdup(command);
		return (full_path);
	}
	dirs = ft_split(ft_getenv(data->env, "PATH"), ':');
	command_length = ft_strlen(command);

	// Checks all path folders to see if it's correct
	i = 0;
	while (dirs[i] != NULL)
	{	
		dprintf(data->log, "Checking directory: %s\n", dirs[i]);
		directory_length = ft_strlen(dirs[i]);
		full_path_size = directory_length + command_length + 2;
		full_path = ft_calloc(full_path_size, sizeof(char));
		if (!full_path)
		{
			perror("Malloc");
			free_str_array(dirs, i);
			clean_up_exit(data, errno, NULL);
			return (NULL);
		}
		ft_strcpy(full_path, dirs[i]);
		ft_strlcat(full_path, "/", full_path_size);
		ft_strlcat(full_path, command, full_path_size);
		dprintf(data->log, "Checking path: %s\n", full_path);
		if (access(full_path, F_OK) == 0)
		{
			dprintf(data->log, "Executable found: %s\n", full_path);
			free_str_array(dirs, i);
			return (full_path);

		}
		free(full_path);
		i++;
	}
	free_str_array(dirs, i);
	ft_dprintf(2, "%s: command not found\n", command);
	ft_dprintf(data->log, "%s: command not found\n", command);
	clean_up_exit(data, 127, NULL);
	return (NULL);
}

int	check_access(char *full_path, t_data *data, t_command *cmd)
{
	struct stat	status_buffer;
	
	// Check it's executable
	if (access(full_path, X_OK) != 0)
	{
		ft_dprintf(2, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		ft_dprintf(data->log, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		free(full_path);
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
		free(full_path);
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
