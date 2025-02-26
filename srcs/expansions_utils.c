/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:16:29 by alize             #+#    #+#             */
/*   Updated: 2025/02/26 17:45:18 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_key(char *line, int i)
{
	int		start;
	char	*key;

	start = i;
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	key = ft_substr(line, start, i - start);
	return (key);
}

void	append_substring(char **result, char *line, int start, int end)
{
	char	*temp;
	char	*new_line;

	if (end > start)
	{
		temp = ft_substr(line, start, end - start);
		new_line = ft_strjoin(*result, temp);
		free(*result);
		free(temp);
		*result = new_line;
	}
}