#include "minishell.h"

int	execute_builtin(char **av, t_data *data);

int	handle_input(char *line, t_data *data)
{
	if (unclosed_quote_detected(line))
		return (FAILURE);

	// 1: GET FIRST TOKENS
	if (tokenise(line, data) == FAILURE)
		return (FAILURE);
	
	// 2: CHECK THE SYNTAX
	if (check_token_syntax(data->tokens_list) == FAILURE)
	{
		token_lst_clear(&data->tokens_list, free);
		return (FAILURE);
	}

	// 2A: GET THE NUMBER OF COMMANDS
	data->command_count = get_command_count(data->tokens_list);

	// 3: EXPAND && REMOVE QUOTES
	handle_expansions(data, data->env);
//
//	// 3A: MANAGE HEREDOCS
//		--> create temp file (unique name)
//		--> replace the delimiter content token with the actual file name, so in fork redirection it opens the file name [<<]->[C] becomes [<<]->[file.txt]

	printf("\n--OUTPUT--\n");	

	// 4: launch if solo builtin

	if (data->command_count == 1)
	{
		int status = launch_solo_command(data);
		printf("command returned with exit status %i\n", status);
	}

	else
		printf("Pipeline of commands detected\n");

	token_lst_clear(&data->tokens_list, free);	
	return (SUCCESS);
}

int	get_command_count(t_token *list)
{
	int	pipe_count;

	pipe_count = 0;
	while (list != NULL)
	{
		if (list->type == PIPE)
			pipe_count++;
		list = list->next;
	}
	return (pipe_count + 1);
}

// Passing data as a parameter as the built in functions will need it
int	execute_builtin(char **av, t_data *data)
{
	(void)data;
	int	status;

	status = 0;

	if (ft_strcmp(av[0], "echo") == 0)
		status = ft_echo(&av[1]);
	else if (ft_strcmp(av[0], "cd") == 0)
		status = ft_cd(&av[1]);
	else if (ft_strcmp(av[0], "pwd") == 0)
		status = ft_pwd();
	return (status);
}
