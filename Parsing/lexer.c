/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:56:54 by caburges          #+#    #+#             */
/*   Updated: 2025/03/17 13:57:03 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	line_is_whitespace(char *line);

int	minishell_lexer(t_data *data, char *line)
{
	if (line_is_whitespace(line))
		return (FAILURE);
	if (unclosed_quote_detected(line))
	{
		ft_dprintf(2, "minishell: unclosed quote detected\n");
		data->status = 1;
		return (FAILURE);
	}
	if (tokenise(line, data) == FAILURE)
	{
		perror("tokenise: malloc failure");
		data->status = 1;
		return (FAILURE);
	}
	return (SUCCESS);
}

// Checks for space, tab and new line characters as per bash MAN
int	line_is_whitespace(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
			return (0);
		i++;
	}
	return (1);
}
