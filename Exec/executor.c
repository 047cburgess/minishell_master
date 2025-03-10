/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:38:47 by caburges          #+#    #+#             */
/*   Updated: 2025/02/28 15:13:26 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell_executor(t_data *data, int cmd_count, t_command *commands)
{
	ft_dprintf(data->log, "\n--OUTPUT--\n");
	if (cmd_count == 1)
	{
		launch_solo_command(data, commands);
		ft_dprintf(data->log, "Command returned with exit status %i\n", data->status);
	}
	else
	{
		launch_pipeline(data, commands, cmd_count);
		ft_dprintf(data->log, "last command returned with exit status %i\n", data->status);
	}
	clean_job_memory(data);
}

int	launch_solo_command(t_data *data, t_command *command)
{
	ft_dprintf(data->log, "FUNCTION: Launch_solo_command\n");
	if (is_builtin(command->av))
		launch_builtin(data, command);
	else
	{
		execute_solo_child(data, command);
		if (command->pid != -1)
		{
			waitpid(command->pid, &data->status, 0);
			data->status = get_child_exit_status(data->status);
		}
	}
	return (data->status);
}

// LINES OK
int	execute_solo_child(t_data *data, t_command *cmd)
{
	cmd->pid = fork();
	if (cmd->pid == -1)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		data->status = 1;
		return (1);
	}
	if (cmd->pid == 0)
	{
		restore_signals_for_child();
		handle_redirections(data, cmd, cmd->fds);
		if (cmd->ac == 0 && cmd->error == 0)
			cmd->error = ER_NO_CMD;
		else if (cmd->av[0][0] == '\0')
			cmd->error = ER_CMD_NOT_FOUND;
		set_command_path(data, cmd->path, cmd->av[0], cmd);
		check_access(cmd->path, data, cmd);
		print_errors_and_exit(data, cmd, CHILD);
		execve(cmd->path, cmd->av, data->env_array);
		ft_dprintf(2, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		close_fds(cmd);
		clean_up_exit(data, errno, NULL);
	}
	return (0);
}
