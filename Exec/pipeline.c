/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:58:01 by caburges          #+#    #+#             */
/*   Updated: 2025/03/12 10:58:03 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	launch_last_child_pipe(t_data *data, t_command *cmd, t_command *prev);
int	launch_middle_child_pipe(t_data *data, t_command *cmd, t_command *prev);
int	launch_first_child_pipe(t_data *data, t_command *cmd);

/* Waits for all the child processes to finish, 
   and captures the value for the last cmd in pipeline
   to update data->status for $? */
int	wait_all_forks(t_data *data, t_command *commands, int num_cmds)
{
	int	i;

	i = 0;
	while (i < num_cmds)
	{
		if ((i == num_cmds - 1) && (commands->pid != -1))
			waitpid(commands->pid, &data->status, 0);
		else if (commands->pid != -1)
			waitpid(commands->pid, NULL, 0);
		commands = commands->next;
		i++;
	}
	data->status = get_child_exit_status(data->status);
	return (0);
}

// all except the last command creates a pipe
int	launch_pipeline(t_data *data, t_command *commands, int num_cmds)
{
	t_command	*current;
	t_command	*prev;
	int			i;

	current = commands;
	i = 0;
	while (i < num_cmds)
	{
		if (i == 0)
			launch_first_child_pipe(data, current);
		else if (i < num_cmds - 1)
			launch_middle_child_pipe(data, current, prev);
		else
			launch_last_child_pipe(data, current, prev);
		if (current->error != 0)
			break ;
		prev = current;
		current = current->next;
		i++;
	}
	print_errors_and_exit(data, current, PARENT);
	wait_all_forks(data, commands, num_cmds);
	return (0);
}

// launch sequence for last command in the pipeline
int	launch_last_child_pipe(t_data *data, t_command *cmd, t_command *prev)
{
	if (!create_fork(cmd))
		return (FAILURE);
	if (cmd->pid == 0)
	{
		restore_signals_for_child();
		connect_last_child_pipe(cmd, prev);
		handle_redirections(cmd, cmd->fds);
		check_no_or_empty_command(cmd);
		set_cmd_path(data, cmd->path, cmd->av[0], cmd);
		check_access(cmd->path, cmd);
		print_errors_and_exit(data, cmd, CHILD);
		if (is_builtin(cmd->av))
			execute_builtin(cmd->av, data, cmd);
		else
		{
			execve(cmd->path, cmd->av, data->env_array);
			ft_dprintf(2, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		}
		if (is_builtin(cmd->av))
			clean_up_exit(data, data->status, NULL);
		clean_up_exit(data, errno, NULL);
	}
	ft_close(&prev->fds[0]);
	ft_close(&prev->fds[1]);
	return (0);
}

// launch sequence for commands in the middle of pipeline
int	launch_middle_child_pipe(t_data *data, t_command *cmd, t_command *prev)
{
	if (!create_pipe(cmd) || !create_fork(cmd))
		return (FAILURE);
	if (cmd->pid == 0)
	{
		restore_signals_for_child();
		connect_middle_child_pipe(cmd->fds, cmd, prev);
		handle_redirections(cmd, cmd->fds);
		check_no_or_empty_command(cmd);
		set_cmd_path(data, cmd->path, cmd->av[0], cmd);
		check_access(cmd->path, cmd);
		print_errors_and_exit(data, cmd, CHILD);
		if (is_builtin(cmd->av))
			execute_builtin(cmd->av, data, cmd);
		else
		{
			execve(cmd->path, cmd->av, data->env_array);
			ft_dprintf(2, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		}
		if (is_builtin(cmd->av))
			clean_up_exit(data, data->status, NULL);
		clean_up_exit(data, errno, NULL);
	}
	ft_close(&prev->fds[0]);
	ft_close(&prev->fds[1]);
	return (0);
}

// launch sequence for last command in a pipeline
int	launch_first_child_pipe(t_data *data, t_command *cmd)
{
	if (!create_pipe(cmd) || !create_fork(cmd))
		return (FAILURE);
	if (cmd->pid == 0)
	{
		restore_signals_for_child();
		connect_first_child_pipe(cmd->fds, cmd);
		handle_redirections(cmd, cmd->fds);
		check_no_or_empty_command(cmd);
		set_cmd_path(data, cmd->path, cmd->av[0], cmd);
		check_access(cmd->path, cmd);
		print_errors_and_exit(data, cmd, CHILD);
		if (is_builtin(cmd->av))
			execute_builtin(cmd->av, data, cmd);
		else
		{
			execve(cmd->path, cmd->av, data->env_array);
			ft_dprintf(2, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		}
		if (is_builtin(cmd->av))
			clean_up_exit(data, data->status, NULL);
		clean_up_exit(data, errno, NULL);
	}
	return (0);
}
