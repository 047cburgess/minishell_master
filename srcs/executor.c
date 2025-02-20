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
// if built in, redirections done in parent, what about heredoc && multiple redirections? still all in parent?
#include <fcntl.h>
#include <unistd.h>

int	handle_redirections(t_data *data, char *command, int *in_out)
{
	t_token *token;
	(void)command;

	in_out[0] = STDIN_FILENO;
	in_out[1] = STDOUT_FILENO;

	token = data->tokens_list;

	while (token->next != NULL && token->type != PIPE)
	{
		if (type_is_redirection(token->type))
		{
			if (token->type == RD_IN)
			{
				printf("trying to redirect in\n");
				if (in_out[0] != STDIN_FILENO)
					close(in_out[0]);
				in_out[0] = open(token->next->content, O_RDONLY);
				if (in_out[0] == -1)
					printf("couldnt open\n");
				printf("opened redirect in\n");
				if (dup2(in_out[0], STDIN_FILENO) == -1)
					perror("couldnt dup stdin");
				close(in_out[0]);
				printf("close infile\n");
			}
			else
			{
				printf("trying to redirect out\n");
				if (in_out[1] != STDOUT_FILENO)
					close(in_out[1]);
				in_out[1] = open(token->next->content, O_RDWR | O_CREAT, 0644);
				if (in_out[1] == -1)
					printf("couldnt create/open file\n");
				printf("opened/created outfile\n");
				if (dup2(in_out[1], STDOUT_FILENO) == -1)
					perror("dup out failed");
				close(in_out[1]);
				printf("closed outfile\n");
			}
		}
		token = token->next;

	}

	return (0);
}

//char **get_argv(t_token *tokens, char *command)
//{}

int	launch_solo_command(t_data *data)
{
	char *command = NULL;
	char *av[] = {"echo", "hello", NULL};

	int	in_out[2];

	command = get_command(data->tokens_list);
	printf("av[0]: %s\n", command);
	// Check if heredoc? or if just one command can it be done in parent?
	// If is a builtin
	if (is_builtin(command))
	{
		handle_redirections(data, command, in_out);
		ft_echo(&av[1]);
		//av = get_argv(data->tokens_list, command);
	
	}

	// Manage redirections
	
	// 
	printf("Here\n");
	return (SUCCESS);
}
/*
int	launch_pipeline(t_data *data, int command_count)
{

}*/
