/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:08:41 by caburges          #+#    #+#             */
/*   Updated: 2025/03/14 18:21:29 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_log = -1;
int	g_signal = 0;

int	main(int ac, char **av, char **envp)
{
	(void)av;
	t_data data;
	char *line;
	ft_bzero(&data, sizeof(t_data));

	data.log = open("log_file.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
	g_log = data.log;
	new_log_timestamp(data.log, "Launched minishell\n");
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
		{
			line = readline("");
		}
		if (line == NULL) // EOF / Ctl+D received
			break;
		add_history(line);
		catch_signals_for_data_status(&data);
		handle_input(line, &data);
		free(line);
	}
	close(data.log);	
	shut_down_minishell(&data);
}

void	new_log_timestamp(int fd, char *message)
{
	time_t now = time(NULL);
	struct tm *tm_info = localtime(&now);
	char buffer[100];
	strftime(buffer, sizeof(buffer), "%H:%M:%S", tm_info);
	dprintf(fd, PINK"[%s] TEST: %s\n"RESET, buffer, message);
}

