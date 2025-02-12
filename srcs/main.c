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

void signal_handler(int signal, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	if (signal == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void init_signals(struct sigaction *act)
{
	// FIRST
	act[0].sa_sigaction = signal_handler;
	sigemptyset(&act[0].sa_mask);
	act[0].sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &act[0], NULL);
	

	// SECOND
	act[1].sa_handler = SIG_IGN;
	sigemptyset(&act[0].sa_mask);
	act[1].sa_flags = 0;
	sigaction(SIGQUIT, &act[1], NULL);
}

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
		if (line == NULL)
		{
			printf("Ctl+D received\n");
			exit(1);
		}
		printf("You enter: %s\n", line);
		add_history(line);
		free(line);
	}	
}
