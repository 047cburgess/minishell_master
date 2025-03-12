/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_command_tables.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:58:11 by caburges          #+#    #+#             */
/*   Updated: 2025/03/12 10:58:12 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// This function gets arrays of env and "PATH", 
// ready to put into command tables for exec
// returns 0 on error (malloc), 1 on success
// sets data->path_dirs to NULL if path has been "unset"
int	set_env_and_path_dir_arrays(t_data *data)
{
	char	*path;

	path = NULL;
	data->env_array = env_to_array(data->env);
	if (data->env_array == NULL)
		return (0);
	// If path is found in env (hasnt been unset), then split it
	path = ft_getenv(data->env, "PATH");
	if (path)
	{
		data->path_dirs = ft_split(path, ':');
		if (!data->path_dirs)
		{
			free_str_array(data->env_array, count_strings(data->env_array));
			free_str_array(data->path_dirs, count_strings(data->path_dirs));
			data->env_array = NULL;
			data->path_dirs = NULL;
			return (0);
		}
	}
	else
		data->path_dirs = NULL;
	return (1);
}

// This function takes the tokens and builds a linked list of command tables
// returns 0 in error (malloc failure), 1 on success
int	prep_command_tables(t_data *data, t_token *tokens)
{
	t_command	*command_tables;

	command_tables = NULL;
	if (!set_env_and_path_dir_arrays(data))
		return (0);
	command_tables = get_command_tables(tokens);
	if (!command_tables)
	{
		free_str_array(data->env_array, count_strings(data->env_array));
		free_str_array(data->path_dirs, count_strings(data->path_dirs));
		data->env_array = NULL;
		data->path_dirs = NULL;
		return (0);
	}
	data->command_list = command_tables;
	return (1);
}

// Returns a pointer to the first token of the next command, 
// or null no more commands
t_token	*get_next_command_start(t_token *prev_cmd_start)
{
	t_token	*token;

	token = prev_cmd_start;
	while (token != NULL && token->type != PIPE)
		token = token->next;
	if (token == NULL)
		return (NULL);
	else
		return (token->next);
}

t_command	*get_command_tables(t_token *tokens)
{
	t_command	*list;
	t_command	*current_command;

	list = NULL;
	current_command = NULL;
	if (!tokens)
		ft_dprintf(g_log, "GET_CMD_TABLES -> tokens is NULL\n");
	while (tokens)
	{
		current_command = new_command_table(tokens);
		if (!current_command)
		{
			command_lst_clear(&list);
			return (NULL);
		}
		command_add_back(&list, current_command);
		tokens = get_next_command_start(tokens);
	}
	print_command_list(list);
	return (list);
}
