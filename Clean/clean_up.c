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

void	clean_up_exit(t_data *data, int exit_status, char *msg)
{
	if (msg)
		printf("%s\n", msg);
	token_lst_clear(&data->tokens_list, free);
	command_lst_clear(&data->command_list);
	env_list_clear(&data->env);
	rl_clear_history();
	close(data->log);
	ft_bzero(data, sizeof(t_data));
	exit(exit_status);
}

void	shut_down_minishell(t_data *data)
{
	// FREE TOKEN LIST
	token_lst_clear(&data->tokens_list, free);
	// FREE CMD TABLE LIST
	command_lst_clear(&data->command_list);
	// CLOSE ANY OPEN FILE DESCRIPTORS
	env_list_clear(&data->env);
	ft_bzero(data, sizeof(t_data));
	rl_clear_history();
}

