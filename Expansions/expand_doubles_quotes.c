/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_doubles_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 18:44:14 by alize             #+#    #+#             */
/*   Updated: 2025/03/17 14:13:30 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_dollar_alone(t_data *data, int *i)
{
	t_list	*new_node;
	char	*dollar;

	dollar = ft_strdup("$");
	if (!dollar)
		return (1);
	new_node = ft_lstnew(dollar);
	if (!new_node)
	{
		free(dollar);
		return (1);
	}
	ft_lstadd_back(&data->cutting, new_node);
	(*i)++;
	return (0);
}

int	handle_static_part(char *line, int start, int end, t_data *data)
{
	t_list	*new_node;
	char	*new_line;

	if (data->expansion_status != 0)
		return (1);
	new_line = ft_substr(line, start, end - start);
	if (!new_line)
	{
		data->expansion_status = 1;
		return (1);
	}
	new_node = ft_lstnew(new_line);
	if (!new_node)
	{
		free(new_line);
		data->expansion_status = 1;
		return (1);
	}
	ft_lstadd_back(&data->cutting, new_node);
	return (0);
}

int	extract_expansion(t_data *data, char *line, int *i, int *start)
{
	if (data->expansion_status != 0)
		return (1);
	handle_expansion(data, line, i);
	*start = *i;
	return (0);
}

int	extract_double_quotes(t_data *data, char *line, int *i)
{
	int	start;

	(*i)++;
	start = *i;
	while (data->expansion_status == 0 && line[*i] && line[*i] != '\"')
	{
		if (line[*i] == '$')
		{
			if (*i > start)
				handle_static_part(line, start, *i, data);
			extract_expansion(data, line, i, &start);
		}
		else
		(*i)++;
	}
	if (*i > start)
		handle_static_part(line, start, *i, data);
	if (line[*i] == '\"')
		(*i)++;
	return (data->expansion_status);
}
