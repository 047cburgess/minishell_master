/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:37:13 by caburges          #+#    #+#             */
/*   Updated: 2025/03/10 15:37:28 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static void signal_handler(int signal, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	g_signal = signal;
	if (signal == SIGINT)
	{
		printf("\n"); // force it to starrt on a new line
		rl_on_new_line(); // tell readline a new line is starting
		rl_replace_line("", 0); // reset cursor to the beginning
		rl_redisplay(); // redisplay the prompt
	}
}

void init_interactive_signals(void)
{
	struct sigaction act[2];

	ft_bzero(&act[0], sizeof(*act));
	ft_bzero(&act[1], sizeof(*act));

	// Ctl + C --> reprompt
	act[0].sa_sigaction = signal_handler;
	sigemptyset(&act[0].sa_mask);
	act[0].sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &act[0], NULL);
	

	// Ctl + \ --> do nothing
	act[1].sa_handler = SIG_IGN; // ignore
	sigemptyset(&act[0].sa_mask);
	act[1].sa_flags = 0;
	sigaction(SIGQUIT, &act[1], NULL);
}

void	heredoc(int signal)
{
	g_signal = signal;
	close(0);
	printf("\n"); // force it to starrt on a new line
	//rl_on_new_line(); // tell readline a new line is starting
	//rl_replace_line("", 0); // reset cursor to the beginning
	//rl_redisplay(); // redisplay the prompt
}

void	set_heredoc_signals(void)
{
	g_signal = 0;
	signal(SIGINT, heredoc);
}

void	set_noninteractive_signals(void)
{
	g_signal = 0;

	// Ignore ctl+c
	signal(SIGINT, SIG_IGN);
	// already ignoring ctl + \ -
}

void	restore_signals_for_child(void)
{
	signal(SIGINT, SIG_DFL);	
	signal(SIGQUIT, SIG_DFL);	
}
