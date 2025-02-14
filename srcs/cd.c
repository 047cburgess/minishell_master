/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:56:19 by caburges          #+#    #+#             */
/*   Updated: 2025/02/13 14:56:21 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Behaviour: Only changes directory for the lifetime of the current process ->
// Many change to represent a mock program --> ft_cd(int ac, char **av) ??
// Then the 'program' is responsible for cleaning up it's own args??
// If more than one argument is given, print too many args
// If no arguments given, go to home: getenv?

int	ft_cd(char **args)
{
	int	ac;

	ac = count_ac(args);
	// If more than one argument is given, return false
	if (ac > 1)
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (0);
	}

	// TODO If no args are given, send home -> need to pass in our env
	if (ac == 0)
		chdir(getenv("HOME")); // temporary using getenv but need to traverse our own path
	// if getenv returns null, perror?

	// If the chdir fails
	else if (chdir(args[0]) == -1)
	{
		perror("minishell: cd : %s"); // Change for strerror?
		return (0);
	}

	// Test
	//ft_pwd();
	return (1);
}
