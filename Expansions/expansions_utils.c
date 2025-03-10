/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:09:32 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/10 14:45:35 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_key(char *line, int i)
{
	int		start;
	char	*key;

	if (!line || i < 0)
		return (NULL);
	start = i;
	if (ft_isdigit(line[i]))
	{
		return (ft_substr(line, i, 1));
	}
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	key = ft_substr(line, start, i - start);
	if (!key)
		return (NULL);
	ft_dprintf(g_log, "%s\n", key);
	return (key);
}

char	*join_list(t_list *lst)
{
	char	*new_line;
	char	*temp;
	t_list	*current;
    t_list	*print;

    ft_dprintf(g_log, "FUNCTION: JOIN LIST\n");
    print = lst;
	ft_dprintf(g_log, "CUTTINGS: ");
    while (print)
    {
	    ft_dprintf(g_log, "[%s]->", (char*)print->content);
	    print = print->next;
    }
    ft_dprintf(g_log, "\n");

	if (!lst)
		return (NULL);
	new_line = ft_strdup("");
	if (!new_line)
		return (NULL);
	current = lst;
	while (current)
	{
		temp = new_line;
		new_line = ft_strjoin(new_line, (char *)current->content);
		if (!new_line)
		{
			free(temp);
			return (NULL);
		}
		free(temp);
		current = current->next;
	}
	return (new_line);
}
