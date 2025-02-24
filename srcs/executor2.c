/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:38:47 by caburges          #+#    #+#             */
/*   Updated: 2025/02/24 14:54:06 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// NEED TO STORE
// 	pids for each process (or built in) to get exit status
//
// solo builtin
//
// solo non-builtin
//
// pipeline
// 	1st in pipe
// 	middle in pipe
// 	last in pipe
//
// T_CMD
// int	pipe_fds[2];
// int	in_out_fds[2];
// char **av;
// int	is_piped
// 
//
//
int	launch_child(t_data *data, t_token *tokens, char **av);

int	launch_solo_command(t_data *data)
{
	char	*command;
	int	ac;
	char	**av;
	int	in_out[2];
	pid_t	pid;
	

	command = get_command(data->tokens_list);
	ac = get_ac(data->tokens_list, data);
	av = get_av(data->tokens_list, ac);

	if (is_builtin(command))
	{
		// MANAGE THAT
		// RETURN THE STATUS
	}
	else
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
		}
		if (pid == 0)
		{
			handle_redirections(data, in_out);
			launch_child(data, data->tokens_list, av);

		}
		if (pid != -1)
			waitpid(pid, NULL, 0);
	}
	return (1);
}

int	launch_child(t_data *data, t_token *tokens, char **av)
{
	char	*path = ft_getenv(data->env, "PATH");
	char	**split_path = ft_split(path, ':');
	char	*full_command_path = get_command_path(data, split_path, av[0]);
	char	**env = env_to_array(data->env);

	close(data->log);
	if (execve(full_command_path, av, env) == -1)
		perror("execve");
	//free_strs_array(split_path);
	free(full_command_path);
	//free_strs_array(env);
	token_lst_clear(&tokens, free);
	close(data->log);
	exit(1);
}
	

	



