/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:03:06 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/11 14:33:50 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_expansion(t_data *data, t_list **cutting, char *line, int *i)
{
	t_list	*new_node;
	
	if (line[*i] == '$' && line[*i + 1] == '?')
		handle_exit_extansion(data, cutting, line, i);
	else if (line[*i] == '$' && line[*i + 1])
	{
		new_node = convert_var_expansion(data, line, i);
		if (new_node)
			ft_lstadd_back(cutting, new_node);
	}
	else if (line[*i] == '$' && line[*i + 1] == '\0')
		handle_dollar_alone(cutting, i);
}

char	*expansion_line(t_data *data, char *line)
{
	t_list	*cutting;
	char	*new_line;
	int		i;

	i = 0;
	cutting = NULL;
	while (line[i])
	{
		if (line[i] == '\'')
			handle_simple_quotes(&cutting, line, &i);
		else if (line[i] == '\"')
			extract_double_quotes(data, &cutting, line, &i);
		else if (line[i] == '$')
			handle_expansion(data, &cutting, line, &i);
		else
			handle_simple_text(&cutting, line, &i);
	}
	new_line = join_list(cutting);
	if (!new_line)
		return (ft_lstclear(&cutting, free), NULL);
	ft_lstclear(&cutting, free);
	return (new_line);
}

char	*expand_token(t_data *data, char *content)
{
	if (!content)
		return (NULL);
	else
		return (expansion_line(data, content));
}

int	handle_expansions_in_tokens(t_data *data)
{
	t_token	*current;
	char	*expanded_content;

	current = data->tokens_list;
	if (!current)
		return (FAILURE);
	ft_dprintf(data->log, "--EXPANSIONS--\n");
	while (current)
	{
		expanded_content = expand_token(data, current->content);
		if (expanded_content != current->content)
			free(current->content);
		current->content = expanded_content;
		ft_dprintf(data->log, "[%s]->", current->content);
		current = current->next;
	}
	ft_dprintf(data->log, "\n");
	return (SUCCESS);
}
