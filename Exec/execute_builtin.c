/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:57:23 by caburges          #+#    #+#             */
/*   Updated: 2025/03/12 10:57:24 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Passing data as a parameter as the built in functions will need it
// NORM OK
int	launch_builtin(t_data *data, t_command *cmd)
{
	int	std_save[2];

	if (!dup_stds(data, std_save))
	{
		cmd->error = 1;
		ft_dprintf(g_log, "LAUNCH_BUILTIN: dup stds failed\n");

	}
	handle_redirections(data, cmd, cmd->fds);
	ft_dprintf(data->log, "preparing to execute %s\n", cmd->av[0]);
	data->status = execute_builtin(cmd->av, data, cmd);
	if (!restore_stds(data, std_save))
	{
		ft_dprintf(g_log, "LAUNCH BUILTIN: failed to restore stds\n");
		cmd->error = 1;
		data->status = cmd->error;
	}
	if (ft_strcmp(cmd->av[0], "exit") == 0 && cmd->error == 0)
		shut_down_minishell(data);
	return (data->status);
}

// NORM OK
int	execute_builtin(char **av, t_data *data, t_command *cmd)
{
	if (cmd->error != 0)
	{
		ft_dprintf(g_log, "EXECUTE BUILTIN: cmd error isnt 0 so not executing\n");
		return (cmd->error);

	}
	if (ft_strcmp(av[0], "echo") == 0)
	{
		data->status = ft_echo(&av[1]);
		ft_dprintf(g_log, "EXECUTE BUILTIN: data status after echo : %i\n", data->status);
	}
	else if (ft_strcmp(av[0], "cd") == 0)
		data->status = ft_cd(&av[1], data);
	else if (ft_strcmp(av[0], "pwd") == 0)
		data->status = ft_pwd();
	else if (ft_strcmp(av[0], "export") == 0)
		data->status = ft_export(av, data);
	else if (ft_strcmp(av[0], "unset") == 0)
		data->status = ft_unset(av, data);
	else if (ft_strcmp(av[0], "env") == 0)
		data->status = ft_env(data);
	else if (ft_strcmp(av[0], "exit") == 0)
		data->status = ft_exit(av, data, cmd);
	return (data->status);
}
