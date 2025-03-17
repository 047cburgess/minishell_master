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

// DO WE NEED TO CATCH SIGPIPE TO FREE IT BUILTIN RECEIVES?

int	catch_signals_for_data_status(t_data *data)
{
	if (g_signal != 0)
	{
		data->status = g_signal + 128;
		g_signal = 0;
		return (1);
	}
	return (0);
}

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
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	heredoc(int signal)
{
	g_signal = signal;
	close(STDIN_FILENO);
	printf("\n"); // force it to starrt on a new line
}

void	set_heredoc_signals(void)
{
	signal(SIGINT, heredoc);
}

void	set_noninteractive_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_signals_for_child(void)
{
	signal(SIGINT, SIG_DFL);	
	signal(SIGQUIT, SIG_DFL);	
}
