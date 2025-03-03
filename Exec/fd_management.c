#include "minishell.h"

void	close_fds(t_command *cmd)
{
	if (cmd->fds[0] > STDIN_FILENO)
		close(cmd->fds[0]);
	if (cmd->fds[1] > STDOUT_FILENO)
		close(cmd->fds[1]);
}

int	dup_stds(t_data *data, int *std_save)
{
	ft_dprintf(data->log, "duping save of stdin stdout\n");
	std_save[0] = dup(STDIN_FILENO);
	std_save[1] = dup(STDOUT_FILENO);
	return (0);
}

int	restore_stds(t_data *data, int *std_save)
{
	ft_dprintf(data->log, "restoring stdin stdout\n");
	dup2(std_save[0], STDIN_FILENO);
	dup2(std_save[1], STDOUT_FILENO);
	ft_dprintf(data->log, "closing dup of stdin stdout\n");
	close(std_save[0]);
	close(std_save[1]);
	return (0);
}

