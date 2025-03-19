/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:38:47 by caburges          #+#    #+#             */
/*   Updated: 2025/02/28 15:13:26 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// executes the commands and frees any memory associated with them when finished
void	minishell_executor(t_data *data, int cmd_count, t_command *commands)
{
	if (cmd_count == 1)
		launch_solo_command(data, commands);
	else
		launch_pipeline(data, commands, cmd_count);
	delete_heredocs_files(data, data->tokens_list);
	clean_job_memory(data);
}

// sequence for solo commands not in a pipeline
// builtin is launched in the parent process
// external commands launched in a child process
int	launch_solo_command(t_data *data, t_command *command)
{
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

// sequence for a solo external command in a fork
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
		handle_redirections(cmd, cmd->fds);
		if (cmd->ac == 0 && cmd->error == 0)
			cmd->error = ER_NO_CMD;
		else if (cmd->av[0][0] == '\0')
			cmd->error = ER_CMD_NOT_FOUND;
		set_cmd_path(data, cmd->path, cmd->av[0], cmd);
		check_access(cmd->path, cmd);
		print_errors_and_exit(data, cmd, CHILD);
		execve(cmd->path, cmd->av, data->env_array);
		ft_dprintf(2, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		close_fds(cmd);
		clean_up_exit(data, errno, NULL);
	}
	return (0);
}

int	create_fork(t_command *cmd)
{
	cmd->pid = fork();
	if (cmd->pid == -1)
	{
		cmd->error = ER_FORK;
		return (FAILURE);
	}
	return (SUCCESS);
}

int	create_pipe(t_command *cmd)
{
	if (pipe(cmd->fds) == -1)
	{
		cmd->error = ER_PIPE;
		return (FAILURE);
	}
	return (SUCCESS);
}
