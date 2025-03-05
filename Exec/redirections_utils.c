/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 18:07:10 by caburges          #+#    #+#             */
/*   Updated: 2025/02/28 18:07:11 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	is_redirection_in(int type)
{
	return (type == RD_IN || type == RD_HEREDOC);
}


int	connect_first_child_pipe(int *fds, t_command *cmd)
{
	//ft_dprintf(2, "first child closing read end[%i]\n", fds[0]);
	ft_close(&fds[0]);
	if (dup2(fds[1], STDOUT_FILENO) == -1)
	{
		cmd->error = ER_FAILED_RD;
		ft_dprintf(2, "minishell: %s: %s\n", strerror(errno));
		return (0);
	}
	//ft_dprintf(2, "first child connected stdout to write end[%i]\n", fds[1]);
	ft_close(&fds[1]);
	return (1);
}

int	connect_middle_child_pipe(int *fds, t_command *cmd, t_command *prev)
{
	//ft_dprintf(2, "middle child closing read end[%i]\n", fds[0]);
	ft_close(&fds[0]);
	if (dup2(fds[1], STDOUT_FILENO) == -1)
	{
		cmd->error = ER_FAILED_RD;
		ft_dprintf(2, "minishell: %s: %s\n", strerror(errno));
		return (0);
	}
	//ft_dprintf(2, "middle child connected stdout to write end[%i]\n", fds[1]);
	ft_close(&fds[1]);
	if (dup2(prev->fds[0], STDIN_FILENO) == -1)
	{
		cmd->error = ER_FAILED_RD;
		ft_dprintf(2, "minishell: %s: %s\n", strerror(errno));
		return (0);
	}
	//ft_dprintf(2, "middle child connected stdin to read end[%i]\n", prev->fds[0]);
	ft_close(&prev->fds[0]);
	ft_close(&prev->fds[1]);
	return (1);
}

int	connect_last_child_pipe(t_command *cmd, t_command *prev)
{
	//ft_dprintf(2, "last child closing write end[%i]\n", prev->fds[1]);
	ft_close(&prev->fds[1]);
	cmd->fds[0] = prev->fds[0];
	if (dup2(cmd->fds[0], STDIN_FILENO) == -1)
	{
		cmd->error = ER_FAILED_RD;
		ft_dprintf(2, "minishell: %s: %s\n", strerror(errno));
		return (0);
	}
	//ft_dprintf(2, "last child connected stdin to read end[%i]\n", cmd->fds[0]);
	ft_close(&cmd->fds[0]);
	return (1);
}
