/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 15:52:44 by caburges          #+#    #+#             */
/*   Updated: 2025/02/17 15:54:20 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Function to free everything we need to when we exit minishell


void	clean_job_memory(t_data *data)
{
	token_lst_clear(&data->tokens_list, free);
	command_lst_clear(&data->command_list);
	free_str_array(data->env_array, count_strings(data->env_array));
	free_str_array(data->path_dirs, count_strings(data->path_dirs));
	data->env_array = NULL;
	data->path_dirs = NULL;
}

void	clean_up_exit(t_data *data, int exit_status, char *msg)
{
	if (msg)
		printf("%s\n", msg);
	clean_job_memory(data);
	env_list_clear(&data->env);
	rl_clear_history();
	close(data->log);
	ft_bzero(data, sizeof(t_data));
	exit(exit_status);
}

void	shut_down_minishell(t_data *data)
{
	clean_job_memory(data);
	// CLOSE ANY OPEN FILE DESCRIPTORS
	env_list_clear(&data->env);
	ft_bzero(data, sizeof(t_data));
	rl_clear_history();
}

