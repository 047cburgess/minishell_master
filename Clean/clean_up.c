/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 15:52:44 by caburges          #+#    #+#             */
/*   Updated: 2025/03/17 18:31:43 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Function to free everything we need to when we exit minishell

void	clean_job_memory(t_data *data)
{
	ft_free((void *)&data->line);
	token_lst_clear(&data->tokens_list, free);
	command_lst_clear(&data->command_list);
	free_str_array(data->env_array, count_strings(data->env_array));
	free_str_array(data->path_dirs, count_strings(data->path_dirs));
	data->env_array = NULL;
	data->path_dirs = NULL;
	data->tokens_list = NULL;
	data->command_list = NULL;
}

void	clean_up_exit(t_data *data, int exit_status, char *msg)
{
	if (msg)
		printf("%s\n", msg);
	clean_job_memory(data);
	env_list_clear(&data->env);
	env_list_clear(&data->export);
	rl_clear_history();
	ft_bzero(data, sizeof(t_data));
	exit(exit_status);
}

void	shut_down_minishell(t_data *data)
{
	int	exit_code;

	if (g_signal == 0)
		exit_code = data->status;
	else
		exit_code = g_signal + 128;
	clean_job_memory(data);
	env_list_clear(&data->env);
	env_list_clear(&data->export);
	ft_bzero(data, sizeof(t_data));
	rl_clear_history();
	exit(exit_code);
}
