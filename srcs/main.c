/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:08:41 by caburges          #+#    #+#             */
/*   Updated: 2025/02/12 18:13:27 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	(void)envp;
	char *line;
	struct sigaction act[2];
	
	init_signals(act);
	while(1)
	{
		line = readline(PROMPT);
		if (line == NULL) // EOF / Ctl+D received
		{
			exit(1); // Temporary
		}
		printf("You entered: %s\n", line);
		add_history(line);
		free(line);
	}	
}
