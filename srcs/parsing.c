#include "minishell.h"

int	execute_builtin(char **av, t_data *data);

// just for testing basic input for builtins, no quotes, no pipes, no redirections

int	handle_input(char *line, t_data *data)
{
	// 1: GET FIRST TOKENS
	tokenise(line, data);

	// 2: CHECK THE SYNTAX
	check_token_syntax(data->tokens_list);
	data->command_count = get_command_count(data->tokens_list);

	// 3: EXPAND && REMOVE QUOTES
		
	// 4: launch if solo command
	if (data->command_count == 1)
		launch_solo_command(data);
//	else
//		launch_pipeline(data, data->command_count);
	// 5: else launch Pipeline


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
	if (ft_strcmp(av[0], "echo") == 0)
		ft_echo(&av[1]);
	else if (ft_strcmp(av[0], "cd") == 0)
		ft_cd(&av[1]);
	else if (ft_strcmp(av[0], "pwd") == 0)
		ft_pwd();
	else
		printf("Not a built in command\n");
	return (SUCCESS);
}
