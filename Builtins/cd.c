/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:56:19 by caburges          #+#    #+#             */
/*   Updated: 2025/03/07 14:28:58 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Behaviour: Only changes directory for the lifetime of the current process ->
// Many change to represent a mock program --> ft_cd(int ac, char **av) ??
// Then the 'program' is responsible for cleaning up it's own args??
// If more than one argument is given, print too many args
// If no arguments given, go to home: getenv?

int	ft_cd(char **av)
{
	int	ac;

	ac = count_ac(av);
	if (ac > 1)
	{
		ft_dprintf(2, "Minishell: cd: too many arguments\n");
		return (1);
	}
	else if (ac == 0)
	{
		ft_dprintf(2, "Minishell: cd: Not enough argument\n");
		return (1);
	}
	else if (chdir(av[0]) == -1)
	{
		ft_dprintf(2, "Minishell: cd: %s: %s\n", av[0], strerror(errno));
		return (1);
	}
	return (0);
}
