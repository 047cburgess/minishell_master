/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_doubles_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 18:44:14 by alize             #+#    #+#             */
/*   Updated: 2025/03/08 12:39:05 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	empty_quotes(t_list **cutting)
{
	char	*empty_line;
	t_list	*new_node;

	if (!(empty_line = ft_strdup("")) || !(new_node = ft_lstnew(empty_line)))
	{
		free(empty_line);
		return (0);
	}
	ft_lstadd_back(cutting, new_node);
	return (1);
}

static void	text_doubles_quotes(t_list **cutting, char *line, int start, int end)
{
	char	*substr;
	t_list	*node;

	if (end > start)
	{
		substr = ft_substr(line, start, end - start);
		if (substr && (node = ft_lstnew(substr)))
			ft_lstadd_back(cutting, node);
		else
			free(substr);
	}
}

void extract_double_quotes(t_data *data, t_list **cutting, char *line, int *i)
{
    int     start;
    t_list  *new_node;

    start = (*i)++;
    while (line[*i] && line[*i] != '\"')
    {
        if (line[*i] == '$' && line[*i + 1] == '?')
        {
            text_doubles_quotes(cutting, line, start, *i);
            handle_exit_extansion(data, cutting, line, i);
            start = *i;
        }
        else if (line[*i] == '$' && line[*i + 1] != '?' && line[*i + 1] != '\"')
        {
            text_doubles_quotes(cutting, line, start, *i);
            new_node = convert_var_expansion(data, line, i);
            ft_lstadd_back(cutting, new_node);
            start = *i;
        }
        else
            (*i)++;
    }
    text_doubles_quotes(cutting, line, start, *i);
    if (line[*i] == '\"')
        (*i)++;
}
