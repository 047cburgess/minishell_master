/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:09:32 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/14 15:33:59 by alsuchon         ###   ########.fr       */
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

char	*join_list(t_list **lst)
{
	char	*new_line;
	char	*temp;
	t_list	*current;

	if (!lst)
		return (NULL);
	new_line = ft_strdup("");
	if (!new_line)
		return (NULL);
	current = *lst;
	while (current)
	{
		temp = new_line;
		ft_dprintf(g_log, "current content = [%s]\n", current->content);
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

char	*heredoc_delim_tkn(char *line)
{
	char	*ptr;
	char	quote;

	ptr = line;
	while (*ptr != '\0')
	{
		if (!is_quote(*ptr))
			ptr++;
		else
		{
			quote = *ptr;
			ft_strcpy(ptr, ptr + 1);
			while (*ptr != quote)
				ptr++;
			ft_strcpy(ptr, ptr + 1);
		}
	}
	return (line);
}
