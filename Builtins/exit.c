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

// to check at school -- seems to not exit sometimes and exits other times.
// returns always between 0 and 255 -> cast to unsigned int
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
			ft_dprintf(2, "minishell: exit: %s: numeric argument required\n", av[1]); // and exits
			data->status = 2;
			return (data->status);
		}
		else if (i > 1)
		{
			ft_dprintf(2, "minishell: exit: too many arguments\n"); // anddoesnt exit
			cmd->error = 1;
			return (1);
		}
		i++;
	}
	return ((unsigned int)ft_atoi(av[1]));
}
