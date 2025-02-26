#include "minishell.h"


// DISABLED THIS SO CAN TEST WITH REAL COMMANDS FIRST

// TODO

// redirections done in parent, need to check about how to manage heredoc

#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

// This function goes through the tokens and processes redirections from left to right
// TODO: Handle errors in file opening
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
					clean_up_exit(data, errno, NULL);

				}
				dprintf(log_file, "Opened '%s'\n", token->next->content);
				if (dup2(in_out[0], STDIN_FILENO) == -1)
				{
					perror("Dup2:");
					clean_up_exit(data, errno, NULL);

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
					clean_up_exit(data, errno, NULL);

				}
				dprintf(log_file, "Redirecting output to '%s'\n", token->next->content);
				if (dup2(in_out[1], STDOUT_FILENO) == -1)
				{
					perror("dup out failed");
					clean_up_exit(data, errno, NULL);
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
	char	**directories;
	int	directory_length;
	int	command_length;
	int	full_path_size;
	int	i;

	// Checks if it's a relative or absolute path already given
	if (ft_strchr(command, '/') != NULL)
	{
		full_path = ft_strdup(command);
		if (access(command, R_OK) == 0)
		{
			dprintf(data->log, "Access OK:\tcmd: %s\n", full_path);

			return (full_path);
		}
		perror("access");
		dprintf(data->log, "Access KO:\tcmd: %s\n", full_path);
		free(full_path);
		clean_up_exit(data, errno, NULL);
		return (NULL);
	}
	directories = ft_split(ft_getenv(data->env, "PATH"), ':');
	command_length = ft_strlen(command);
	// Checks all path folders to see if it's correct
	i = 0;
	while (directories[i] != NULL)
	{	
		dprintf(data->log, "Checking directory: %s\n", directories[i]);
		directory_length = ft_strlen(directories[i]);
		full_path_size = directory_length + command_length + 2;
		full_path = ft_calloc(full_path_size, sizeof(char));
		if (!full_path)
		{
			perror("Malloc");
			free_str_array(directories, i);
			clean_up_exit(data, errno, NULL);
			return (NULL);
		}
		ft_strcpy(full_path, directories[i]);
		ft_strlcat(full_path, "/", full_path_size);
		ft_strlcat(full_path, command, full_path_size);
		dprintf(data->log, "Checking path: %s\n", full_path);
		if (access(full_path, R_OK) == 0)
		{
			dprintf(data->log, "Executable found: %s\n", full_path);
			free_str_array(directories, i);
			return (full_path);

		}
		free(full_path);
		i++;
	}
	free_str_array(directories, i);
	clean_up_exit(data, 127, "Command not found");
	return (NULL);
}


// Function to process a simple command
// BUILTIN, CHILD, TBD how to incorporate heredoc in the handle redirections
/*int	launch_solo_command1(t_data *data)
{
	int	log_file = data->log;
	char	*command;
	int	ac;
	char	**av;

	command = get_command(data->tokens_list);
	ac = get_ac(data->tokens_list);
	av = get_av(data->tokens_list, ac);
	

	int	std[2];
	int	in_out[2];

	std[0] = dup(STDIN_FILENO);
	std[1] = dup(STDOUT_FILENO);
	handle_redirections(data, in_out);
	
	// Check if heredoc? or if just one command can it be done in parent?
	if (is_builtin(command))
	{
		dprintf(log_file, "PARENT: identified builtin command [%s]\n", command);
		dprintf(log_file, "PARENT: attempting to execute [%s]\n", command);
		execute_builtin(av, data);
		//av = get_argv(data->tokens_list, command);
		if (in_out[1] != STDOUT_FILENO)
		{
			dup2(std[1], STDOUT_FILENO);
			close(std[1]);
		}
		if (in_out[0] != STDIN_FILENO)
		{
			dup2(std[0], STDIN_FILENO);
			close(std[0]);
		}	
		//	NEED TO BREAK OUT SO DOESNT TRY TO FORK AND EXECVE AFTER
	}
	int	pid;

	dprintf(log_file, "PARENT: command [%s] is not built in, will attempt to find command\n", command);
	dprintf(log_file, "PARENT: preparing to fork into child\n");
	pid = fork();
	if (pid == -1)
	{
		dprintf(log_file, "PARENT: fork failed\n"); 
		perror("Fork failed");
		close(std[0]);
		close(std[1]);
		return (FAILURE);
	}
	if (pid == 0)
	{
		dprintf(log_file, "\tENTERED CHILD\n");
		if (in_out[1] != STDOUT_FILENO)
		{
			dup2(in_out[1], STDOUT_FILENO);
			close(in_out[1]);
		}
		if (in_out[0] != STDIN_FILENO)
		{
			dup2(in_out[0], STDIN_FILENO);
			close(in_out[0]);
		}	
		char	*path = ft_getenv(data->env, "PATH");
		char	**split_path = ft_split(path, ':');
		char	*command_path = get_command_path(data, split_path, command);
		dprintf(log_file, "\t\tCHILD: attempting to exec %s\n", av[0]);
		if (execve(command_path, av, env_to_array(data->env)) == -1)
			perror("execve");
		dprintf(log_file, "\t\tCHILD: execve failed\n");
		dprintf(log_file, "\t\tCHILD: exiting shell\n");
		exit(1);
	}
	dprintf(log_file, "PARENT: waiting for child '%s'...\n", av[0]);
	waitpid(pid, NULL, 0);

	dprintf(log_file, "PARENT: resetting std[0] std[1]\n");
        dup2(std[0], STDIN_FILENO);
        dup2(std[1], STDOUT_FILENO);

	dprintf(log_file, "PARENT: closing std[0],std[1]\n");
        close(std[0]);
        close(std[1]);
	dprintf(log_file, "PARENT: leaving execute solo command function\n");
	return (SUCCESS);
}*/
/*
int	launch_pipeline(t_data *data, int command_count)
{

}*/
