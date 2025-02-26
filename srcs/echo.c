/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:45:05 by caburges          #+#    #+#             */
/*   Updated: 2025/02/26 13:24:29 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_ac(char **args)
{
	int	ac;

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
		return (FALSE);
	if (ft_strcmp(args[0], "-n") == 0)
		return (TRUE);
	else
		return (FALSE);
}

int	ft_echo(char **args)
{
	int	i;

	i = 0;
	if (option(args) == TRUE)
		i++;
	while (args[i] != NULL)
	{
		printf("%s", args[i]);
		// if (args[i + 1] != NULL)
		// 	printf(" ");
		i++;
	}
	if (option(args) == FALSE)
		printf("\n");
	return (1);
}