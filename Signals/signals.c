/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:37:13 by caburges          #+#    #+#             */
/*   Updated: 2025/03/10 19:44:57 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void signal_handler(int signal)
{
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
	// Ctl + C --> reprompt
	signal(SIGINT, signal_handler);
	
	// Ctl + \ --> do nothing
	signal(SIGQUIT, SIG_IGN);
}

void	heredoc(int signal)
{
	g_signal = signal;
	close(0);
	ft_dprintf(g_log, "ctrl C detected.. exit heredocs\n");
	printf("\n");
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