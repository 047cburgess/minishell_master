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

void	shut_down_minishell(t_data *data)
{
	env_list_clear(&data->env);
	rl_clear_history();
}
