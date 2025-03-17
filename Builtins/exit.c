/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:58:48 by caburges          #+#    #+#             */
/*   Updated: 2025/03/14 16:28:17 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **av, t_data *data, t_command *cmd)
{
	int	i;

	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		ft_dprintf(2, "exit\n");
	if (count_ac(av) == 1)
		return (data->status);
	i = 1;
	while (av[i])
	{
		if (!is_numeric(av[i]) && i == 1)
		{
			ft_dprintf(2, ER_FT_EXIT_VAL, av[1]);
			data->status = 2;
			return (data->status);
		}
		else if (i > 1)
		{
			ft_dprintf(2, ER_FT_EXIT_ARG);
			cmd->error = 1;
			return (1);
		}
		i++;
	}
	return ((unsigned int)ft_atoi(av[1]));
}
