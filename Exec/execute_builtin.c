#include "minishell.h"

// Passing data as a parameter as the built in functions will need it


int	launch_builtin(t_data *data, t_command *cmd)
{
	int	std_save[2];
	
	dup_stds(data, std_save);
	handle_redirections(data, cmd, cmd->fds);
	dprintf(data->log, "preparing to execute %s\n", cmd->av[0]);
	data->status = execute_builtin(cmd->av, data, cmd);
	restore_stds(data, std_save);
	if (ft_strcmp(cmd->av[0], "exit") == 0 && cmd->error == 0)
		shut_down_minishell(data);
	return (data->status);
}

int	execute_builtin(char **av, t_data *data, t_command *cmd)
{
	if (cmd->error != 0)
		return (cmd->error);
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
	else if (ft_strcmp(av[0], "exit") == 0)
		data->status = ft_exit(av, data);
	return (data->status);
}
