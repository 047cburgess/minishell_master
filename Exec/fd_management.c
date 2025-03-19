/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_management.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:57:52 by caburges          #+#    #+#             */
/*   Updated: 2025/03/12 10:57:54 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// safely closes a set of file of in out file descriptors
void	close_fds(t_command *cmd)
{
	if (cmd->fds[0] > STDIN_FILENO)
		ft_close(&cmd->fds[0]);
	if (cmd->fds[1] > STDOUT_FILENO)
		ft_close(&cmd->fds[1]);
}

// creates a copy of the original stdin and stdout so they can be restored after redirection
int	dup_stds(int *std_save)
{
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

// restores the original stdin and stdout
int	restore_stds(int *std_save)
{
	int	error;

	error = 1;
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
	ft_close(&std_save[0]);
	ft_close(&std_save[1]);
	return (error);
}

// closes all open fds for the whole pipeline
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

void	ft_close(int *fd)
{
	if (*fd != -1)
		close(*fd);
	*fd = -1;
}
