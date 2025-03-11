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

void    handle_empty_quotes(t_data *data, int *i)
{
    t_list  *new_node;
    char    *empty;

    empty = ft_strdup("");
    if (!empty)
        return ;
    new_node = ft_lstnew(empty);
    if (!new_node)
        return;
    ft_lstadd_back(&data->cutting, new_node);
    (*i)++;
}

void    handle_static_part(char *line, int start, int end, t_data *data)
{
    t_list *new_node;
    char *new_line;
    
    new_line = ft_substr(line, start, end - start);
    if (!new_line)
        return;
    new_node = ft_lstnew(new_line);
    if (!new_node)
    {
        free(new_line);
        return;
    }
    ft_lstadd_back(&data->cutting, new_node);
}

void extract_expansion(t_data *data, char *line, int *i, int *start)
{
    handle_expansion(data, line, i);
    *start = *i;
}

void    extract_double_quotes(t_data *data, char *line, int *i)
{
    int start;
    
    (*i)++;
    start = *i;
    if (line[*i] == '\"')
    {
        handle_empty_quotes(data, i);
        return;
    }
    while (line[*i] && line[*i] != '\"')
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
}
