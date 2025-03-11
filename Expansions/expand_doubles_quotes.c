/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_doubles_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 18:44:14 by alize             #+#    #+#             */
/*   Updated: 2025/03/11 14:43:47 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void extract_double_quotes(t_data *data, t_list **cutting, char *line, int *i)
{
    int start;
    char *new_line;
    t_list *new_node;

    (*i)++;
    start = *i;
    if (line[*i] == '\"')
    {
        new_node = ft_lstnew(ft_strdup(""));
        if (!new_node)
            return ;
        ft_lstadd_back(cutting, new_node);
        (*i)++;
        return ;
    }
    while (line[*i] && line[*i] != '\"')
    {
        if (line[*i] == '$')
        {
            if (*i > start)
            {
                new_line = ft_substr(line, start, *i - start);
                if (!new_line)
                    return;
                new_node = ft_lstnew(new_line);
                if (!new_node)
                {
                    free(new_line);
                    return;
                }
                ft_lstadd_back(cutting, new_node);
            }
            handle_expansion(data, cutting, line, i);

            start = *i;
        }
        else
            (*i)++;
    }
    if (*i > start)
    {
        new_line = ft_substr(line, start, *i - start);
        if (!new_line)
            return;
        new_node = ft_lstnew(new_line);
        if (!new_node)
        {
            free(new_line);
            return;
        }
        ft_lstadd_back(cutting, new_node);
    }
    if (line[*i] == '\"')
        (*i)++; 
}
