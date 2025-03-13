/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_doubles_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 18:44:14 by alize             #+#    #+#             */
/*   Updated: 2025/03/11 17:22:20 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int    handle_empty_quotes(t_data *data, int *i)
{
    t_list  *new_node;
    char    *empty;

    empty = ft_strdup("");
    if (!empty)
        //return (1);
	data->expansion_status = 1;
    new_node = NULL; //ft_lstnew(empty);
    if (!new_node)
	{
		free(empty);
        	return (1);
	}
    ft_lstadd_back(&data->cutting, new_node);
    (*i)++;
    return (0);
}

int    handle_static_part(char *line, int start, int end, t_data *data)
{
    t_list *new_node;
    char *new_line;
    
    if (data->expansion_status != 0)
	    return (1);
    new_line = ft_substr(line, start, end - start);
    if (!new_line)
        return (1);
    new_node = ft_lstnew(new_line);
    if (!new_node)
    {
        free(new_line);
        return (1);
    }
    ft_lstadd_back(&data->cutting, new_node);
    return (0);
}

void extract_expansion(t_data *data, char *line, int *i, int *start)
{
	if (data->expansion_status != 0)
		return ;
    handle_expansion(data, line, i);
    *start = *i;
}

int    extract_double_quotes(t_data *data, char *line, int *i)
{
    int start;
    
    (*i)++;
    start = *i;
    if (line[*i] == '\"')
        return (handle_empty_quotes(data, i));
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
