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
#include <unistd.h>
#include "ft_dprintf.h"


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

int	dup_stds(t_data *data, int *std_save)
{
	ft_dprintf(data->log, "duping save of stdin stdout\n");
	std_save[0] = dup(STDIN_FILENO);
	std_save[1] = dup(STDOUT_FILENO);
	return (0);
}

int	launch_solo_command(t_data *data, t_command *command)
{
	int	std_save[2];
	if (is_builtin(command->av))
	{
		ft_dprintf(data->log, "cmd identified as builtin\n");
		dup_stds(data, std_save);
		if (handle_redirections(data, command, command->fds) == 0)
		{
			dprintf(data->log, "preparing to execute %s\n", command->av[0]);
			data->status = execute_builtin(command->av, data);
		}
		else
			data->status = 1;
		restore_stds(data, std_save);
	}
	else
	{
		execute_solo_child(data, command);
		if (command->pid != -1)
		{
			waitpid(command->pid, &data->status, 0);
			data->status = get_child_exit_status(data->status);
		}
	}
	command_lst_clear(&data->command_list);
	return (data->status);
}

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
		handle_redirections(data, cmd, cmd->fds);
		if (cmd->ac == 0 && cmd->error == 0)
			cmd->error = ER_NO_CMD;
		set_command_path(data, cmd->path, cmd->av[0], cmd);
		check_access(cmd->path, data, cmd);
		print_errors_and_exit(data, cmd);
		execve(cmd->path, cmd->av, cmd->env);
		ft_dprintf(2, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		close_fds(cmd);
		clean_up_exit(data, 1, NULL);
	}
	return (0);
}
