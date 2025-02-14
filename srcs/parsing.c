#include "minishell.h"

int	execute_builtin(t_command *cmd);

// just for testing basic input for builtins, no quotes, no pipes, no redirections
// Haven't managed any memory protections etc. 
int	handle_input(char *line, t_data *data)
{
	t_command	*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	cmd->argv = ft_split(line, ' ');
	cmd->cmd = cmd->argv[0];
	cmd->env = data->env;
	data->command = cmd;
	execute_builtin(data->command);
	// Clean up the memory
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
