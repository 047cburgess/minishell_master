/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:37:13 by caburges          #+#    #+#             */
/*   Updated: 2025/02/13 11:03:39 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void signal_handler(int signal, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	if (signal == SIGINT)
	{
		printf("\n"); // force it to starrt on a new line
		rl_on_new_line(); // tell readline a new line is starting
		rl_replace_line("", 0); // reset cursor to the beginning
		rl_redisplay(); // redisplay the prompt
	}
}

void init_signals(struct sigaction *act)
{
	// Ctl + C --> reprompt
	ft_bzero(&act[0], sizeof(*act));
	act[0].sa_sigaction = signal_handler;
	sigemptyset(&act[0].sa_mask);
	act[0].sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &act[0], NULL);
	

	// Ctl + \ --> do nothing
	ft_bzero(&act[1], sizeof(*act));
	act[1].sa_handler = SIG_IGN; // ignore
	sigemptyset(&act[0].sa_mask);
	act[1].sa_flags = 0;
	sigaction(SIGQUIT, &act[1], NULL);
}
