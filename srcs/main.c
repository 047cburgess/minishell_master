/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:08:41 by caburges          #+#    #+#             */
/*   Updated: 2025/02/19 16:16:47 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	(void)av;
	t_data data;
	char *line;
	struct sigaction act[2];

	data.log = open("log_file.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
	new_log_timestamp(data.log, "Testing append to the log file");
	if (ac != 1)
	{
		ft_putendl_fd("Minishell doesn't take any arguments", 2);
		return (1);
	}
	init_signals(act);
	if (!set_environment(envp, &data))
		return (1);
	while(1)
	{
		line = readline(PROMPT);
		if (line == NULL) // EOF / Ctl+D received
			break;
		add_history(line);
		handle_input(line, &data);
		free(line);
	}
	close(data.log);	
	shut_down_minishell(&data);
	exit(5);
}

void	new_log_timestamp(int fd, char *message)
{
	time_t now = time(NULL);
	struct tm *tm_info = localtime(&now);
	char buffer[100];
	strftime(buffer, sizeof(buffer), "%H:%M:%S", tm_info);
	dprintf(fd, PINK"[%s] TEST: %s\n"RESET, buffer, message);
}

