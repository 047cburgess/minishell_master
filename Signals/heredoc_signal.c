/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:27:26 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/17 14:28:06 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc(int signal)
{
	g_signal = signal;
	close(STDIN_FILENO);
	printf("\n");
}

void	set_heredoc_signals(void)
{
	signal(SIGINT, heredoc);
}
