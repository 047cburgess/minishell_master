#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "ft_dprintf.h"

// This function goes through the tokens and processes redirections from left to right
int	handle_redirections(t_data *data, t_command *cmd, int *in_out)
{
	int log_file = data->log;
	t_token *token;

	token = cmd->tokens;
	while (token->next != NULL && token->type != PIPE)
	{
		dprintf(log_file, "[%s] entering handle redirection\n", token->content);
		if (type_is_redirection(token->type))
		{
			dprintf(log_file, "[%s] identified as redirection\n", token->content);
			if (token->type == RD_IN)
			{
				dprintf(log_file, "[%s] identified as RD_IN\n", token->content);
				in_out[0] = open(token->next->content, O_RDONLY);
				if (in_out[0] == -1)
				{
					perror("Open:");
					return (errno);
				}
				dprintf(log_file, "Opened '%s'\n", token->next->content);
				if (dup2(in_out[0], STDIN_FILENO) == -1)
				{
					perror("Dup2:");
					return (errno);

				}
				if (in_out[0] != STDIN_FILENO)
					close(in_out[0]);
				dprintf(log_file, "Closed '%s'\n", token->next->content);
			}
			else
			{
				dprintf(log_file, "[%s] identified as RD_OUT\n", token->content);
				in_out[1] = open(token->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (in_out[1] == -1)
				{
					perror("Open:");
					return (errno);

				}
				dprintf(log_file, "Redirecting output to '%s'\n", token->next->content);
				if (dup2(in_out[1], STDOUT_FILENO) == -1)
				{
					perror("dup out failed");
					return (errno);
				}
				dprintf(log_file, "Closing '%s'\n", token->next->content);
				if (in_out[1] != STDOUT_FILENO)
				{
					close(in_out[1]);
					dprintf(log_file, "Closed '%s'\n", token->next->content);

				}
			}
		}
		token = token->next;
	}
	return (0);
}

				

// This function returns the full path for a given command, ready to give to execve
// If it's not found, it returns NULL
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
	ft_dprintf(2, "minishell: %s: command not found\n", command);
	ft_dprintf(data->log, "minishell: %s: command not found\n", command);
	clean_up_exit(data, 127, NULL);
	return (NULL);
}
