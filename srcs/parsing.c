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

	printf("\n--OUTPUT--\n");		
	// 4: launch if solo command
	if (data->command_count == 1)
		launch_solo_command(data);
	else
		printf("Pipeline of commands detected\n");
//		launch_pipeline(data, data->command_count);
	// 5: else launch Pipeline

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
	if (ft_strcmp(av[0], "echo") == 0)
		ft_echo(&av[1]);
	else if (ft_strcmp(av[0], "cd") == 0)
		ft_cd(&av[1]);
	else if (ft_strcmp(av[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(av[0], "export") == 0)
		ft_export(av, data);
	else if (ft_strcmp(av[0], "unset") == 0)
		ft_unset(av, data);
	else if (ft_strcmp(av[0], "env") == 0)
		ft_env(data);
	else
		printf("Not a built in command\n");
	return (SUCCESS);
}
