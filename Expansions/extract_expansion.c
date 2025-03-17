/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:03:06 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/14 15:28:10 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_expansion(t_data *data, char *line, int *i)
{
	t_list	*new_node;

	if (data->expansion_status != 0)
		return (1);
	if (line[*i] == '$' && line[*i + 1] == '?')
		data->expansion_status = handle_exit_extansion(data, line, i);
	else if (line[*i] == '$' && line[*i + 1])
	{
		new_node = convert_var_expansion(data, line, i);
		if (new_node)
			ft_lstadd_back(&data->cutting, new_node);
		else
			data->expansion_status = 1;
	}
	else if (line[*i] == '$' && line[*i + 1] == '\0')
		data->expansion_status = handle_dollar_alone(data, i);
	return (data->expansion_status);
}

char	*expansion_line(t_data *data, char *line)
{
	char	*new_line;
	int		i;
	int		status;

	data->cutting = NULL;
	i = 0;
	status = 0;
	while (status == 0 && line[i])
	{
		if (line[i] == '\'')
			status = handle_simple_quotes(data, line, &i);
		else if (line[i] == '\"')
			status = extract_double_quotes(data, line, &i);
		else if (line[i] == '$')
			status = handle_expansion(data, line, &i);
		else
			status = handle_simple_text(data, line, &i);
	}
	if (status == 0)
		new_line = join_list(&data->cutting);
	else
		return (ft_lstclear(&data->cutting, free), NULL);
	ft_lstclear(&data->cutting, free);
	return (new_line);
}

char	*expand_token(t_data *data, char *content)
{
	if (!content)
		return (NULL);
	else
		return (expansion_line(data, content));
}

// Perror and clean if fail
int	handle_expansions_in_tokens(t_data *data)
{
	t_token	*current;
	char	*expanded_content;

	current = data->tokens_list;
	if (!current)
		return (FAILURE);
	while (current)
	{
		if (current->type == RD_HEREDOC)
		{
			current->next->content = heredoc_delim_tkn(current->next->content);
			current = current->next->next;
			continue ;
		}
		expanded_content = expand_token(data, current->content);
		if (!expanded_content)
		{
			perror("expander: malloc failure");
			return (FAILURE);
		}
		free(current->content);
		current->content = expanded_content;
		current = current->next;
	}
	return (SUCCESS);
}
