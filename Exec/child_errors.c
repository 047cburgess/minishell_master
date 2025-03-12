/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:56:45 by caburges          #+#    #+#             */
/*   Updated: 2025/03/12 10:56:47 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// NORM OK
int	print_errors_and_exit(t_data *data, t_command *command, int mode)
{
	if (!command || command->error == 0)
		return (0);
	if (command->error == ER_CMD_NOT_FOUND)
	{
		ft_dprintf(2, "minishell: %s: command not found\n", command->av[0]);
		command->error = 127;
	}
	else if (command->error == ER_IS_DIR)
	{
		ft_dprintf(2, "minishell: %s: Is a directory\n", command->av[0]);
		command->error = 126;
	}
	else if (command->error == ER_NO_CMD)
		command->error = 0;
	else if (command->error != ER_FAILED_RD)
		ft_dprintf(2, "minishell: %s: %s\n", command->av[0], strerror(errno));
	close_all_fds(data);
	if (mode == CHILD)
		clean_up_exit(data, command->error, NULL);
	else if (mode == PARENT && command->error == 1)
		data->status = 1;
	return (1);
}
