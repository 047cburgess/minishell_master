/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:30:22 by alize             #+#    #+#             */
/*   Updated: 2025/02/24 11:27:02 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_dprintf.h"

int	get_after_next_quote(char *start)
{
	int		i;
	char	quote;

	i = 1;
	quote = start[0];
	while (start[i] != quote)
		i++;
	i++;
	return (i);
}

// line too long, but wll just shorten name of is_operator function...
t_token	*get_quoted_token(char *ptr)
{
	t_token	*new_token;
	char	*new_content;
	int		i;

	i = 0;
	while (1)
	{
		if (is_quote(ptr[i]))
			i += get_after_next_quote(&ptr[i]);
		if (ptr[i] == '\0' || ft_isspace(ptr[i]) || is_operator(ptr[i]))
			break;
		while (ptr[i] && !is_quote(ptr[i]) && !ft_isspace(ptr[i]) && !is_operator(ptr[i]))
				i++;
	}
	new_content = ft_substr(ptr, 0, i);
	if (!new_content)
		return (NULL);
	new_token = new_token_node(new_content);
	if (!new_token)
	{
		free(new_content);
		return (NULL);
	}
	return (new_token);
}

t_token	*get_unquoted_token(char *start)
{
	char	*ptr;
	int		len;
	t_token	*new_token;
	char	*new_content;

	ptr = start;
	while (*ptr && !is_operator(*ptr) && *ptr != ' ')
	{
		if (is_quote(*ptr))
			ptr = ft_strchr(ptr + 1, *ptr) + 1;
		else
			ptr++;
	}
	len = ptr - start;
	new_content = ft_substr(start, 0, len);
	if (!new_content)
		return (NULL);
	new_token = new_token_node(new_content);
	if (!new_token)
		return (free(new_content), NULL);
	return (new_token);
}

int	get_token_type(char *content)
{
	if (ft_strcmp(content, "<<") == 0)
		return (RD_HEREDOC);
	else if (ft_strcmp(content, ">>") == 0)
		return (RD_APPEND);
	else if (ft_strcmp(content, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(content, "<") == 0)
		return (RD_IN);
	else
		return (RD_OUT);
}

t_token	*get_operator_token(char *start)
{
	char	*content;
	t_token	*token;

	if (ft_strncmp(start, "<<", 2) == 0)
		content = ft_strdup("<<");
	else if (ft_strncmp(start, ">>", 2) == 0)
		content = ft_strdup(">>");
	else if (*start == '|')
		content = ft_strdup("|");
	else if (*start == '<')
		content = ft_strdup("<");
	else
		content = ft_strdup(">");
	if (!content)
		return (NULL);
	token = new_token_node(content);
	if (!token)
	{
		free(content);
		return (NULL);
	}
	token->type = get_token_type(content);
	return (token);
}

int	tokenise(char *line, t_data *data)
{
	int		i;

	i = 0;
	while (line[i] != '\0')
	{
		while (ft_isspace(line[i]))
			i++;
		if (is_operator(line[i]))
		{
			if (!handle_operator_token(&i, &line[i], &data->tokens_list))
				return (token_lst_clear(&data->tokens_list, free), FAILURE);
		}
		else if (is_quote(line[i]))
		{
			if (!handle_quoted_token(&i, &line[i], &data->tokens_list))
				return (token_lst_clear(&data->tokens_list, free), FAILURE);
		}
		else if (!is_quote(line[i]) && line[i] != '\0')
		{
			if (!handle_unquoted_token(&i, &line[i], &data->tokens_list))
				return (token_lst_clear(&data->tokens_list, free), FAILURE);
		}
	}
	ft_dprintf(data->log, "--TOKENS--\n\t");
	print_tokens_list(data->log, data->tokens_list);
	return (SUCCESS);
}
