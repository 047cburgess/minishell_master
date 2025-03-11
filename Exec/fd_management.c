#include "minishell.h"

// NORM OK
void	close_fds(t_command *cmd)
{
	if (cmd->fds[0] > STDIN_FILENO)
		ft_close(&cmd->fds[0]);
	if (cmd->fds[1] > STDOUT_FILENO)
		ft_close(&cmd->fds[1]);
}

// NORM OK
int	dup_stds(t_data *data, int *std_save)
{
	ft_dprintf(data->log, "duping save of stdin stdout\n");
	std_save[0] = dup(STDIN_FILENO);
	if (std_save[0] == -1)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		return (0);
	}
	std_save[1] = dup(STDOUT_FILENO);
	if (std_save[1] == -1)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		ft_close(&std_save[0]);
		return (0);
	}
	return (1);
}

// NORM OK
int	restore_stds(t_data *data, int *std_save)
{
	int	error;

	error = 1;
	ft_dprintf(data->log, "restoring stdin stdout\n");
	if (dup2(std_save[0], STDIN_FILENO) == -1)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		error = 0;
	}
	if (dup2(std_save[1], STDOUT_FILENO) == -1)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		error = 0;
	}
	ft_dprintf(data->log, "closing dup of stdin stdout\n");
	ft_close(&std_save[0]);
	ft_close(&std_save[1]);
	return (error);
}

// NORM OK
void	close_all_fds(t_data *data)
{
	t_command	*current;

	current = data->command_list;
	while (current)
	{
		close_fds(current);
		current = current->next;
	}
}

// NORM OK
void	ft_close(int *fd)
{
	if (*fd != -1)
		close(*fd);
	*fd = -1;
}
