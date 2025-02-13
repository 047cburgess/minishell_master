/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:45:05 by caburges          #+#    #+#             */
/*   Updated: 2025/02/13 12:04:16 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int 	count_ac(char **args)
{
	int ac;

	ac = 0;
	while (args[ac] != NULL)
	{
		ac++;
	}
	return (ac);
}

bool	option(int ac, char **args)
{

	if (args[0] == NULL)
		return (false);
	ac = 6;
	if (ft_strcmp(args[0], "-n") == 0)
		return (true);
	else
		return (false);
}

int	ft_echo(char **args)
{
	int	i;
	int ac;

	i = 0;
	ac = count_ac(args);
	if (option(ac, args) == true)
		i++;
	while (args[i] != NULL)
	{
		printf("%s", args[i]);
		if (args[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (option(ac, args) == false)
		printf("\n");
	return (1);
}
