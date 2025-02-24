/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:16:29 by alize             #+#    #+#             */
/*   Updated: 2025/02/24 14:50:33 by alize            ###   ########.fr       */
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

int dollar_count(char *line, int *i)
{
	int	count;

	count = 0;
	while (line[*i] == '$')
	{
		count++;
		(*i)++;
	}
	return (count);
}
