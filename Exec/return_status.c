/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_status.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:58:35 by caburges          #+#    #+#             */
/*   Updated: 2025/03/12 10:58:36 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// NORM OK
// If it exited normally, retrieve its exit code
// If it exited by a signal, return the signal num + 128
int	get_child_exit_status(int status)
{
	int	exit_signal;

	exit_signal = 0;
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		exit_signal = WTERMSIG(status);
		if (exit_signal == 3)
			printf("Quit (core dumped)");
		printf("\n");
		return (128 + exit_signal);
	}
	else
		return (1);
}
