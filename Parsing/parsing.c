#include "minishell.h"
#include "ft_dprintf.h"

int	execute_builtin(char **av, t_data *data);

int	handle_input(char *line, t_data *data)
{
	set_noninteractive_signals();
	new_log_timestamp(data->log, line);
	if (line[0] == '\0')
	{
		data->status = 0;
		return (FAILURE);

	}
	if (unclosed_quote_detected(line))
		return (FAILURE);

	// 1: GET FIRST TOKENS
	if (tokenise(line, data) == FAILURE)
		return (FAILURE);
	
	// 2: CHECK THE SYNTAX
	if (check_token_syntax(data->tokens_list) == FAILURE)
	{
		data->status = 2;
		token_lst_clear(&data->tokens_list, free);
		return (FAILURE);
	}

	// 2A: GET THE NUMBER OF COMMANDS
	data->command_count = get_command_count(data->tokens_list);

	// 3: EXPAND && REMOVE QUOTES
	handle_expansions(data);
//
//	// 3A: MANAGE HEREDOCS
//		--> create temp file (unique name)
//		--> replace the delimiter content token with the actual file name, so in fork redirection it opens the file name [<<]->[C] becomes [<<]->[file.txt]

	//prep_heredocs(data, data->tokens_list)
	prep_command_tables(data, data->tokens_list);

	// 4: launch if solo builtin

	ft_dprintf(data->log, "\n--OUTPUT--\n");
	if (data->command_count == 1)
	{
		launch_solo_command(data, data->command_list);
		ft_dprintf(data->log, "Command returned with exit status %i\n", data->status);
	}
	else
	{
		launch_pipeline(data, data->command_list, data->command_count);
		ft_dprintf(data->log, "last command returned with exit status %i\n", data->status);
	}
	clean_job_memory(data);
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
	if (ft_strcmp(av[0], "echo") == 0)
		data->status = ft_echo(&av[1]);
	else if (ft_strcmp(av[0], "cd") == 0)
		data->status = ft_cd(&av[1]);
	else if (ft_strcmp(av[0], "pwd") == 0)
		data->status = ft_pwd();
	else if (ft_strcmp(av[0], "export") == 0)
		data->status = ft_export(av, data);
	else if (ft_strcmp(av[0], "unset") == 0)
		data->status = ft_unset(av, data);
	else if (ft_strcmp(av[0], "env") == 0)
		data->status = ft_env(data);
	return (data->status);
}
