#include "minishell.h"
#include <sys/wait.h>
#include <sys/stat.h>

int	set_path_for_abs_or_rel_command(t_command *cmd, char *path, char *command)
{
	dprintf(g_log, "Has '/' in command name or path is unset\n");
	ft_strlcpy(path, command, FULL_PATH_MAX);
	ft_dprintf(g_log, "Checking access F_OK (if it exists)\n");
	if (access(path, F_OK != 0))
	{
		ft_dprintf(g_log, "KO -> file no exist, set cmd->error to 127\n");
		cmd->error = 127;
	}
	else
		ft_dprintf(g_log, "OK -> file exists\n");
	return (1);
}

// NORM OK when logs removed
// This function returns the full path for a given cmd (check file exists only)
int	set_command_path(t_data *data, char *path, char *command, t_command *cmd)
{
	int	i;
	
	ft_bzero(path, FULL_PATH_MAX);
	dprintf(data->log, "FUNCTION: GET_COMMAND_PATH\n");
	if (cmd->error != 0 || is_builtin(cmd->av))
	{
		ft_dprintf(data->log, "Builtin/previous Error detected, leaving.\n");
		return (0);
	}
	
	// Checks if it's a relative or absolute path already given, and if that file exists
	if (ft_strchr(command, '/') || data->path_dirs == NULL)
		return (set_path_for_abs_or_rel_command(cmd, path, command));

	// Checks all path folders to see if it's correct
	ft_dprintf(g_log, "Attempting to search PATH_DIRS for file...\n");
	i = 0;
	while (data->path_dirs[i] != NULL)
	{	
		ft_strlcpy(path, data->path_dirs[i], FULL_PATH_MAX);
		ft_strlcat(path, "/", FULL_PATH_MAX);
		ft_strlcat(path, command, FULL_PATH_MAX);
		dprintf(data->log, "Checking path: %s\n", path);
		if (access(path, F_OK) == 0)
		{
			ft_dprintf(data->log, "Executable found: %s\n", path);
			return (1);
		}
		i++;
	}
	cmd->error = ER_CMD_NOT_FOUND;
	ft_dprintf(data->log, "%s: command not found\n", command);
	return (0);
}

// NORM OK WHEN LOGS REMOVED
// Checks the remaining executable permissions -> if it's a directory and if it's executable
// Early return if there was already an error or if it's a builtin
int	check_access(char *full_path, t_data *data, t_command *cmd)
{
	struct stat	status_buffer;

	ft_dprintf(data->log, "FUNC: CHECK_ACCESS\n");
	if (cmd->error != 0 || is_builtin(cmd->av))
		return (0);
	
	// Check it's not a directory
	dprintf(data->log, "--checking if it's a directory\n");
	stat(full_path, &status_buffer);
	if ((status_buffer.st_mode & S_IFMT) == S_IFDIR)
	{
		ft_dprintf(data->log, "--minishell: %s: Is a directory\n", cmd->av[0]);
		cmd->error = ER_IS_DIR;
		return (0);
	}
	else
		dprintf(data->log, "--%s is not a directory\n", cmd->av[0]);

	// Check it's executable
	ft_dprintf(data->log, "--checking access X_OK\n");;
	if (access(full_path, X_OK) != 0)
	{
		ft_dprintf(data->log, "--KO (not executable)-> returning 126: minishell: %s: %s\n", cmd->av[0], strerror(errno));
		cmd->error = 126;
		return (0);
	}
	dprintf(data->log, "--%s is executable\n", cmd->av[0]);
	return (SUCCESS);
}
