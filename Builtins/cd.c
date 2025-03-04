/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:56:19 by caburges          #+#    #+#             */
/*   Updated: 2025/03/04 17:08:01 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Behaviour: Only changes directory for the lifetime of the current process ->
// Many change to represent a mock program --> ft_cd(int ac, char **av) ??
// Then the 'program' is responsible for cleaning up it's own args??
// If more than one argument is given, print too many args
// If no arguments given, go to home: getenv?


// NEEDS TO RETURN THE EXIT STATUS
int	ft_cd(char **args)
{
	int	ac;
	int	status;

	status = 0;
	ac = count_ac(args);
	// If more than one argument is given, return false
	if (ac > 1)
	{
		
		ft_putendl_fd("Minishell: cd: too many arguments", 2);
		status = 1;
	}

	// TODO If no args are given, send home -> need to pass in our env
	else if (ac == 0)
	{
		chdir(getenv("HOME")); // temporary using getenv but need to traverse our own path
		status = 0; // make sure to check if it succeeded and change status value
	}
	// if getenv returns null, perror?

	// If the chdir fails
	else if (chdir(args[0]) == -1)
	{
		perror("minishell: cd : %s"); // Change for strerror?
		status = 1;
		
	}
	return (status);
}
