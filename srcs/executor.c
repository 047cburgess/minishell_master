#include "minishell.h"

char	*get_command(t_token *list)
{
	t_token	*current;
	t_token *prev = NULL;

	current = list;
	if (current->type == WORD)
		return (current->content);
	prev = current;
	current = current->next;
	while (current != NULL && current->type != PIPE)
	{	
		if (current->type == WORD && prev->type == WORD)
			return (current->content);
		prev = current;
		current = current->next;
	}
	return (NULL);
}

int	is_builtin(char *command)
{
	if (command == NULL)
		return (FALSE);
	if (ft_strcmp(command, "echo") == 0)
		return (TRUE);
	if (ft_strcmp(command, "cd") == 0)
		return (TRUE);
	if (ft_strcmp(command, "pwd") == 0)
		return (TRUE);
	else
		return (FALSE);
}

// TODO
// Redirection protocol for solo command
// if not built in, everything done in child
// redirections done in parent, need to check about how to manage heredoc

#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

// This function goes through the tokens and processes redirections from left to right
int	handle_redirections(t_data *data, char *command, int *in_out)
{
	int log_file = data->log;
	t_token *token;
	(void)command;

	in_out[0] = STDIN_FILENO;
	in_out[1] = STDOUT_FILENO;

	token = data->tokens_list;

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
					perror("Open:");
				dprintf(log_file, "Opened '%s'\n", token->next->content);
				if (dup2(in_out[0], STDIN_FILENO) == -1)
					perror("Dup2:");
				close(in_out[0]);
				dprintf(log_file, "Closed '%s'\n", token->next->content);
			}
			else
			{
				dprintf(log_file, "[%s] identified as RD_OUT\n", token->content);
				in_out[1] = open(token->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (in_out[1] == -1)
					perror("Open:");
				dprintf(log_file, "Redirecting output to '%s'\n", token->next->content);
				if (dup2(in_out[1], STDOUT_FILENO) == -1)
					perror("dup out failed");
				dprintf(log_file, "Closing '%s'\n", token->next->content);
				close(in_out[1]);
				dprintf(log_file, "Closed '%s'\n", token->next->content);
			}
		}
		token = token->next;

	}

	return (0);
}

int	get_ac(t_token *command_list, t_data *data)
{
	(void)data;
	int	ac = 0;

	t_token *current;
	t_token *prev = NULL;

	current = command_list;
	if (current->type == WORD)
	{
		ac++;
	}
	prev = current;
	current = current->next;
	while (current != NULL && current->type != PIPE)
	{
		if (current->type == WORD && prev->type == WORD)
		{
			ac++;
		}
		prev = current;
		current = current->next;
	}
	return (ac);
}
				
char	**get_av(t_token *tokens, int ac)
{
	char	**av;
	t_token *current;
	t_token *prev = NULL;
	int i = 0;

	av = ft_calloc((ac + 1), sizeof(char *));
	if (!av)
		return (NULL);

	current = tokens;
	if (current->type == WORD)
	{
		av[i] = ft_strdup(current->content);
		if (!av[i])
			return (free_str_array(av, i), NULL);
		i++;
	}
	prev = current;
	current = current->next;
	while (current != NULL && current->type != PIPE)
	{
		if (current->type == WORD && prev->type == WORD)
		{
			av[i] = ft_strdup(current->content);
			if (!av[i])
				return (free_str_array(av, i), NULL);
			i++;
		}
		prev = current;
		current = current->next;
	}
	return (av);
}

// Function to process a simple command
// BUILTIN, CHILD, TBD how to incorporate heredoc in the handle redirections
// Very ugly for now, just manages commands from /usr/bin just to test functionality with redirections
int	launch_solo_command(t_data *data)
{
	int	log_file = data->log;
	char *command = get_command(data->tokens_list);
	int	ac = get_ac(data->tokens_list, data);
	char **av = get_av(data->tokens_list, ac);
	char path[200];

	ft_strcpy(path, "/usr/bin/");
	ft_strlcat(path, command, sizeof(path));
	
	int	std[2];
	int	in_out[2];

	std[0] = dup(STDIN_FILENO);
	std[1] = dup(STDOUT_FILENO);
	// Check if heredoc? or if just one command can it be done in parent?
	if (is_builtin(command))
	{
		dprintf(log_file, "PARENT: identified builtin command [%s]\n", command);
		handle_redirections(data, command, in_out);
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
	}
	int	pid;

	dprintf(log_file, "PARENT: command [%s] is not built in, will attempt to find command\n", command);
	handle_redirections(data, command, in_out);
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
		dprintf(log_file, "\t\tCHILD: attempting to exec %s\n", av[0]);
		if (!execve(path, av, env_to_array(data->env)))
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
}
/*
int	launch_pipeline(t_data *data, int command_count)
{

}*/
