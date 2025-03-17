/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:57:36 by caburges          #+#    #+#             */
/*   Updated: 2025/03/17 13:57:48 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_dprintf.h"

// Point of control between lexer, parser and executor
int	handle_input(char *line, t_data *data)
{
	set_noninteractive_signals();
	if (!minishell_lexer(data, line))
		return (FAILURE);
	if (!minishell_parser(data))
	{
		token_lst_clear(&data->tokens_list, free);
		return (FAILURE);
	}
	minishell_executor(data, data->command_count, data->command_list);
	return (SUCCESS);
}

int	minishell_parser(t_data *data)
{
	if (check_token_syntax(data->tokens_list) == FAILURE)
	{
		data->status = 2;
		return (FAILURE);
	}
	data->command_count = get_command_count(data->tokens_list);
	if (handle_expansions_in_tokens(data) == FAILURE)
	{
		data->status = 1;
		return (FAILURE);
	}
	if (handle_heredocs(data, data->tokens_list) == FAILURE)
	{
		delete_heredocs_files(data, data->tokens_list);
		if (catch_signals_for_data_status(data))
			return (FAILURE);
		data->status = 1;
		return (perror("heredocs: fail"), FAILURE);
	}
	if (prep_command_tables(data, data->tokens_list) == FAILURE)
	{
		data->status = 1;
		return (FAILURE);
	}
	return (SUCCESS);
}

int	get_command_count(t_token *list)
{
	int	pipe_count;

	pipe_count = 0;
	while (list != NULL)
	{
		if (list->type == PIPE)
			pipe_count++;
		list = list->next;
	}
	return (pipe_count + 1);
}
