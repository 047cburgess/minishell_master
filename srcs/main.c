/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:08:41 by caburges          #+#    #+#             */
/*   Updated: 2025/02/18 15:24:58 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <sys/wait.h>	
	
	

int	main(int ac, char **av, char **envp)
{
	(void)av;
	t_data data;
	char *line;
	struct sigaction act[2];

	if (ac != 1)
	{
		ft_putendl_fd("Minishell doesn't take any arguments", 2);
		return (1);
	}
	init_signals(act);
	if (!set_environment(envp, &data))
		return (1);
	while(1)
	{
		line = readline(PROMPT);
		if (line == NULL) // EOF / Ctl+D received
			break;
		add_history(line);
		parse_and_execute(line, &data);
		//handle_input(line, &data);
		free(line);
	}	
	shut_down_minishell(&data);
	exit(5);
}
