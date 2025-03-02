#include "minishell.h"

int	print_errors_and_exit(t_data *data, t_command *command)
{
	if (command->error == 0)
		return (0);
	if (command->error == 127)
		ft_dprintf(2, "minishell: %s: command not found\n", command->av[0]);
	else if (command->error == ER_IS_DIR)
	{
		ft_dprintf(2, "minishell: %s: Is a directory\n", command->av[0]);
		command->error = 126;
	}
	else if (command->error == ER_NO_CMD)
		command->error = 0;
	else if (command->error != ER_FAILED_RD) 
		ft_dprintf(2, "minishell: %s: %s\n", command->av[0], strerror(errno));
	close_fds(command);
	clean_up_exit(data, command->error, NULL);
	return (1);
}

