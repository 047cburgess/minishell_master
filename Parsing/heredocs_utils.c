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

int	handle_simple_hd_text(t_data *data, char *line, int *i)
{
	char	*new_line;
	t_list	*new_node;
	int		start;

	start = *i;
	while (line[*i] && line[*i] != '$')
	{
		(*i)++;
	}
	new_line = ft_substr(line, start, *i - start);
	if (!new_line)
		return (1);
	new_node = ft_lstnew(new_line);
	if (!new_node)
	{
		free(new_line);
		return (1);
	}
	ft_lstadd_back(&data->cutting, new_node);
	return (0);
}

// returns a line of text after expansions
char	*get_hd_line(char *line, t_data *data)
{
	char	*new_line;
	int		i;
	int		status;

	data->cutting = NULL;
	i = 0;
	status = 0;
	while (status == 0 && line[i])
	{
		if (line[i] == '$')
			status = handle_expansion(data, line, &i);
		else
			status = handle_simple_hd_text(data, line, &i);
	}
	if (status == 0)
		new_line = join_list(&data->cutting);
	else
		return (ft_lstclear(&data->cutting, free), NULL);
	ft_lstclear(&data->cutting, free);
	return (new_line);
}
