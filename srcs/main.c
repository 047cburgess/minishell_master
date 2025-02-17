/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:08:41 by caburges          #+#    #+#             */
/*   Updated: 2025/02/17 18:47:46 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	(void)av;
	t_data data;
	t_token *head;
	char *line;
	struct sigaction act[2];

	
	// Check args
	if (ac != 1)
	{
		ft_putendl_fd("Minishell doesn't take any arguments", 2);
		return (1);
	}
	init_signals(act);
	set_environment(envp, &data);
	while(1)
	{
		line = readline(PROMPT);
		if (line == NULL) // EOF / Ctl+D received
		{
			exit(1); // Temporary exit -> will be builtin
		}
		//printf("You entered: %s\n", line);
		add_history(line);
		//handle_input(line, &data);
		head = extract_tokens(line);
		while (head)
		{
			printf("[%s] ", head->content);
			head = head->next;
		}
		free(line);
	}	
}
