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

int	launch_solo_command(t_data *data)
{
	t_command	*command_table;
	int	std_save[2];
	int	status;

	command_table = new_command_table(data->tokens_list, data);
	if (!command_table)
	{
		data->status = 1;
		return (data->status);
	}
	data->command_list = command_table;
	if (is_builtin(command_table->av))
	{
		ft_dprintf(data->log, "cmd identified as builtin\n");
		dprintf(data->log, "duping save of stdin stdout\n");
		std_save[0] = dup(STDIN_FILENO);
		std_save[1] = dup(STDOUT_FILENO);
		if (handle_redirections(data, command_table, command_table->fds) == 0)
		{
			dprintf(data->log, "preparing to execute %s\n", command_table->av[0]);
			data->status = execute_builtin(command_table->av, data);
		}
		else
			data->status = command_table->error;
		dprintf(data->log, "restoring stdin stdout\n");
		dup2(std_save[0], STDIN_FILENO);
		dup2(std_save[1], STDOUT_FILENO);
		dprintf(data->log, "closing dup of stdin stdout\n");
		close(std_save[0]);
		close(std_save[1]);
	}
	else
	{
		data->status = execute_solo_child(data, command_table);
		if (command_table->pid != -1)
		{
			waitpid(command_table->pid, &status, 0);
			data->status = get_child_exit_status(status);
		}
	}
	command_lst_clear(&data->command_list);
	return (data->status);
}


int	execute_solo_child(t_data *data, t_command *cmd)
{
	char	*full_cmd_path;

	cmd->pid = fork();
	if (cmd->pid == -1)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		return (1);
	}
	if (cmd->pid == 0)
	{
		if (handle_redirections(data, cmd, cmd->fds) != 0)
		{
			close_fds(cmd);
			clean_up_exit(data, cmd->error, NULL);
		}
		if (cmd->ac == 0)
		{
			close_fds(cmd);
			clean_up_exit(data, 0, "No cmds to execute, returned 0");
		}
		full_cmd_path = get_command_path(data, cmd->av[0]);
		ft_dprintf(data->log, "Full cmd path: %s\n", full_cmd_path);
		check_access(full_cmd_path, data, cmd);
		execve(full_cmd_path, cmd->av, cmd->env);
		ft_dprintf(2, "minishell: %s, execution failed\n", cmd->av[0]);
		free(full_cmd_path);
		close_fds(cmd);
		clean_up_exit(data, 1, NULL);
	}
	return (0);
}

