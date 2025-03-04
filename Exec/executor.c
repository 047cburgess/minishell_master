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
		print_errors_and_exit(data, cmd, CHILD);
		execve(cmd->path, cmd->av, data->env_array);
		ft_dprintf(2, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		close_fds(cmd);
		clean_up_exit(data, errno, NULL);
	}
	return (0);
}
