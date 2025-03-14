/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:45:05 by caburges          #+#    #+#             */
/*   Updated: 2025/03/14 12:09:53 by alsuchon         ###   ########.fr       */
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

bool	option(char **args, int *index)
{
	int	i;
	int	j;

	i = 0;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		i++;
	}
	*index = i;
	if (i > 0)
		return (true);
	return (false);
}

int	ft_echo(char **args)
{
	int		i;
	bool	no_newline;

	i = 0;
	no_newline = option(args, &i);
	while (args[i] != NULL)
	{
		printf("%s", args[i]);
		if (args[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (!no_newline)
		printf("\n");
	return (0);
}
