/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:09:32 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/05 10:11:54 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_key(char *line, int i)
{
    int		start;
    char	*key;

	start = i;
	if (ft_isdigit(line[i]))
	{
		return (ft_substr(line, i, 1));

	}
    while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
        i++;
    key = ft_substr(line, start, i - start);
    return (key);
}

char	*join_list(t_list *lst)
{
    char	*new_line;
	char 	*temp;
    t_list	*current;
	
	new_line = ft_strdup("");
	current = lst;
    while (current)
    {
        temp = new_line;
        new_line = ft_strjoin(new_line, (char *)current->content);
        free(temp);
        current = current->next;
    }
    return (new_line);
}