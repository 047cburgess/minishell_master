/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:45:05 by caburges          #+#    #+#             */
/*   Updated: 2025/02/24 17:14:33 by alize            ###   ########.fr       */
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

bool	option(char **args)
{
	if (args[0] == NULL)
		return (false);
	if (ft_strcmp(args[0], "-n") == 0)
		return (true);
	else
		return (false);
}

int	ft_echo(char **args)
{
	int	i;

	i = 0;
	if (option(args) == true)
		i++;
	while (args[i] != NULL)
	{
		printf("%s", args[i]);
		if (args[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (option(args) == false)
		printf("\n");
	return (1);
}
