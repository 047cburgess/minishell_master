/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 14:07:37 by caburges          #+#    #+#             */
/*   Updated: 2025/03/13 14:07:38 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_heredoc_count(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == RD_HEREDOC)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

t_token	*get_next_heredoc_delimiter(t_token *tokens)
{
	while (tokens && tokens->type != RD_HEREDOC)
		tokens = tokens->next;
	if (tokens == NULL)
		return (NULL);
	else
		return (tokens->next);
}

int	delete_heredocs_files(t_data *data, t_token *tokens)
{
	int		i;
	t_token	*current_delimiter;

	current_delimiter = tokens;
	i = 0;
	while (i < data->heredoc_count)
	{
		current_delimiter = get_next_heredoc_delimiter(current_delimiter);
		unlink(current_delimiter->content);
		i++;
		current_delimiter = current_delimiter->next;
	}
	data->heredoc_count = 0;
	return (1);
}
