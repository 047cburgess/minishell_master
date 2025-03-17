/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:37:13 by caburges          #+#    #+#             */
/*   Updated: 2025/03/17 14:27:51 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	signal_handler(int signal)
{
	g_signal = signal;
	if (signal == SIGINT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	init_interactive_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

// Ignore ctl+c | already ignoring ctl + \ -
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
