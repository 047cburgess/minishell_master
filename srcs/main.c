/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:08:41 by caburges          #+#    #+#             */
/*   Updated: 2025/02/19 16:16:47 by alize            ###   ########.fr       */
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
	char *new_line = NULL;
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
		new_line = expansion_line(data.env, line);
		printf("new_line = %s\n", new_line);
		parse_and_execute(new_line, &data);
		free(line);
		free(new_line);
	}	
	shut_down_minishell(&data);
	exit(5);
}
