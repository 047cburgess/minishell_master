/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:08:41 by caburges          #+#    #+#             */
/*   Updated: 2025/03/17 18:18:14 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

int	init_minishell(t_data *data, int ac, char **av, char **envp)
{
	(void)av;
	ft_bzero(data, sizeof(t_data));
	if (ac != 1)
	{
		ft_putendl_fd("Minishell doesn't take any arguments", 2);
		return (0);
	}
	if (!set_environment(envp, data))
		return (0);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;

	if (!init_minishell(&data, ac, av, envp))
		return (EXIT_FAILURE);
	while (1)
	{
		init_interactive_signals();
		data.line = readline(PINK PROMPT RESET);
		if (data.line == NULL)
			break ;
		add_history(data.line);
		catch_signals_for_data_status(&data);
		handle_input(data.line, &data);
		ft_free((void *)&data.line);
	}
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		ft_dprintf(2, "exit\n");
	shut_down_minishell(&data);
}
