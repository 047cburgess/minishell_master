/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:38:47 by caburges          #+#    #+#             */
/*   Updated: 2025/02/24 14:54:06 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

// NEED TO STORE
// 	pids for each process (or built in) to get exit status
//
// solo builtin
//
// solo non-builtin
//
// pipeline
// 	1st in pipe
// 	middle in pipe
// 	last in pipe

#include <sys/stat.h>

int	launch_solo_command(t_data *data)
{
	t_command	*command_table;
	int	status;

	command_table = new_command_table(data->tokens_list, data);
	if (!command_table)
		return (FAILURE);
	print_command_list(command_table);
	data->command_list = command_table;
	if (is_builtin(command_table->av))
	{
		// Handle redirections
		// Execute it and return the status??
	}
	else
		execute_solo_child(data, command_table);
	if (command_table->pid != -1)
		waitpid(command_table->pid, &status, 0);
	command_lst_clear(&data->command_list);
	return (0);
}

int	check_access(char *full_path, t_data *data, t_command *cmd)
{
	struct stat	status_buffer;
	
	// Check it's executable
	if (access(full_path, X_OK) != 0)
	{
		free(full_path);
		perror("Open:");
		close_fds(cmd);
		clean_up_exit(data, errno, NULL);
	}

	// Check it's not a directory
	stat(full_path, &status_buffer);
	if ((status_buffer.st_mode & S_IFMT) == S_IFDIR)
	{
		free(full_path);
		close_fds(cmd);
		clean_up_exit(data, 126, "Is a directory");
	}
	return (SUCCESS);
}

int	execute_solo_child(t_data *data, t_command *cmd)
{
	char	*full_cmd_path;

	cmd->pid = fork();
	if (cmd->pid == -1)
		return (FAILURE);
	if (cmd->pid == 0)
	{
		handle_redirections(data, cmd, cmd->fds);
		print_command_list(cmd);
		if (cmd->ac == 0)
		{
			close_fds(cmd);
			clean_up_exit(data, 0, "No cmds to execute, returned 0");
		}
		full_cmd_path = get_command_path(data, cmd->av[0]);
		check_access(full_cmd_path, data, cmd);
		execve(full_cmd_path, cmd->av, cmd->env);
		perror("execve");
		free(full_cmd_path);
		close_fds(cmd);
		clean_up_exit(data, 2, NULL);
	}
	return (SUCCESS);
}

void	close_fds(t_command *cmd)
{
	if (cmd->fds[0] != STDIN_FILENO)
		close(cmd->fds[0]);
	if (cmd->fds[1] != STDOUT_FILENO)
		close(cmd->fds[1]);
}
