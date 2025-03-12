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

// further clean into norm but already looking farly good and clean
int	launch_last_child_pipe(t_data *data, t_command *cmd, t_command *prev);
int	launch_middle_child_pipe(t_data *data, t_command *cmd, t_command *prev);
int	launch_first_child_pipe(t_data *data, t_command *cmd);

// NORM OK
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

// all except the last command creates a pipe (if not a solo command)
int	launch_pipeline(t_data *data, t_command *commands, int num_cmds)
{
	t_command	*current;
	t_command	*prev;
	int	i;

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
	//data->status = get_child_exit_status(data->status);
	return (0);
}

void	check_no_or_empty_command(t_command *cmd)
{
	if (cmd->ac == 0 && cmd->error == 0)
		cmd->error = ER_NO_CMD;
	else if (cmd->av[0][0] == '\0')
		cmd->error = ER_CMD_NOT_FOUND;
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

int	launch_last_child_pipe(t_data *data, t_command *cmd, t_command *prev)
{
	if (!create_fork(cmd))
		return (FAILURE);
	if (cmd->pid == 0)
	{
		restore_signals_for_child();
		connect_last_child_pipe(cmd, prev);
		handle_redirections(data, cmd, cmd->fds);
		check_no_or_empty_command(cmd);
		set_command_path(data, cmd->path, cmd->av[0], cmd);
		check_access(cmd->path, data, cmd);
		print_errors_and_exit(data, cmd, CHILD);
		if (is_builtin(cmd->av))
			execute_builtin(cmd->av, data, cmd);
		else
		{
			execve(cmd->path, cmd->av, data->env_array);
			ft_dprintf(2, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		}
		close_fds(cmd);
		if (is_builtin(cmd->av))
			clean_up_exit(data, data->status, NULL);
		clean_up_exit(data, errno, NULL);
	}
	ft_close(&prev->fds[0]);
	ft_close(&prev->fds[1]);
	return (0);
}

int	launch_middle_child_pipe(t_data *data, t_command *cmd, t_command *prev)
{
	if (!create_pipe(cmd) || !create_fork(cmd))
		return (FAILURE);
	if (cmd->pid == 0)
	{
		restore_signals_for_child();
		connect_middle_child_pipe(cmd->fds, cmd, prev);
		handle_redirections(data, cmd, cmd->fds);
		check_no_or_empty_command(cmd);
		set_command_path(data, cmd->path, cmd->av[0], cmd);
		check_access(cmd->path, data, cmd);
		print_errors_and_exit(data, cmd, CHILD);
		if (is_builtin(cmd->av))
			execute_builtin(cmd->av, data, cmd);
		else
		{
			execve(cmd->path, cmd->av, data->env_array);
			ft_dprintf(2, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		}
		close_fds(cmd);
		if (is_builtin(cmd->av))
			clean_up_exit(data, data->status, NULL);
		clean_up_exit(data, errno, NULL);
	}
	ft_close(&prev->fds[0]);
	ft_close(&prev->fds[1]);
	return (0);
}

int	launch_first_child_pipe(t_data *data, t_command *cmd)
{
	if (!create_pipe(cmd) || !create_fork(cmd))
		return (FAILURE);
	if (cmd->pid == 0)
	{
		restore_signals_for_child();
		connect_first_child_pipe(cmd->fds, cmd);
		handle_redirections(data, cmd, cmd->fds);
		check_no_or_empty_command(cmd);
		set_command_path(data, cmd->path, cmd->av[0], cmd);
		check_access(cmd->path, data, cmd);
		print_errors_and_exit(data, cmd, CHILD);
		if (is_builtin(cmd->av))
			execute_builtin(cmd->av, data, cmd);
		else
		{
			execve(cmd->path, cmd->av, data->env_array);
			ft_dprintf(2, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		}
		close_fds(cmd);
		if (is_builtin(cmd->av))
			clean_up_exit(data, data->status, NULL);
		clean_up_exit(data, errno, NULL);
	}
	return (0);
}
