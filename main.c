/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:08:41 by caburges          #+#    #+#             */
/*   Updated: 2025/03/17 14:43:26 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

int	main(int ac, char **av, char **envp)
{
	t_data	data;
	char	*line;
	
	(void)av;
	ft_bzero(&data, sizeof(t_data));
	if (ac != 1)
	{
		ft_putendl_fd("Minishell doesn't take any arguments", 2);
		return (1);
	}
	if (!set_environment(envp, &data))
		return (1);
	while(1)
	{
		init_interactive_signals();
		if (isatty(STDIN_FILENO))
			line = readline(PINK PROMPT RESET);
		else
			line = readline("");
		if (line == NULL)
			break ;
		add_history(line);
		catch_signals_for_data_status(&data);
		handle_input(line, &data);
		free(line);
	}
	shut_down_minishell(&data);
}
