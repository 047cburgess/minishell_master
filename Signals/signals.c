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

int	catch_signals_for_data_status(t_data *data)
{
	if (g_signal != 0)
	{
		ft_dprintf(g_log, "caugh sigint in mainloop after readline\n");
		data->status = g_signal + 128;
		g_signal = 0;
		return (1);
	}
	return (0);
}

static void signal_handler(int signal)
{
	g_signal = signal;
	ft_dprintf(g_log, "SIGNAL HANDLER: sigint received\n");
	if (signal == SIGINT)
	{
		ft_dprintf(g_log, "SIGNAL HANDLER: reputting the new prompt\n");
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
	ft_dprintf(g_log, "HEREDOC HANDLER: sigint received\n");
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
	// Ignore ctl+c
	signal(SIGINT, SIG_IGN);
	// already ignoring ctl + \ -
	signal(SIGQUIT, SIG_IGN);
}

void	restore_signals_for_child(void)
{
	signal(SIGINT, SIG_DFL);	
	signal(SIGQUIT, SIG_DFL);	
}
