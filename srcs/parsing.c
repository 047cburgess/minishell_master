#include "minishell.h"

int	execute_builtin(t_command *cmd);

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


	printf("back\n");
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
	printf("Number of commands: %i\n", pipe_count + 1);
	return (pipe_count + 1);
}





int	handle_input2(char *line, t_data *data)
{
	t_command	*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	cmd->argv = ft_split(line, ' ');
	cmd->cmd = cmd->argv[0];
	cmd->env = data->bash_env;
	data->command = cmd;
	//print_str_array(cmd->argv);
	//print_str_array(cmd->env);
	execute_builtin(data->command);
	// Clean up the memory
	free_str_array(cmd->argv, count_strings(cmd->argv));
	free(cmd);
	return (SUCCESS);
}

int	execute_builtin(t_command *cmd)
{
	if (ft_strcmp(cmd->cmd, "echo") == 0)
		ft_echo(&cmd->argv[1]);
	else if (ft_strcmp(cmd->cmd, "cd") == 0)
		ft_cd(&cmd->argv[1]);
	else if (ft_strcmp(cmd->cmd, "pwd") == 0)
		ft_pwd();
	else
		printf("Not a built in command\n");
	return (SUCCESS);
}
