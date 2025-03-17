/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:30:22 by alize             #+#    #+#             */
/*   Updated: 2025/03/14 16:44:25 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_dprintf.h"

// NORM OK
t_token	*get_word_token(char *start)
{
	char	*ptr;
	int		len;
	t_token	*new_token;
	char	*new_content;

	ptr = start;
	while (*ptr && !is_operator(*ptr) && !ft_isspace(*ptr))
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

// NORM OK
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

// NORM OK
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

// NORM OK
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
		else if (line[i] != '\0')
		{
			if (!handle_word_token(&i, &line[i], &data->tokens_list))
				return (token_lst_clear(&data->tokens_list, free), FAILURE);
		}
	}
	ft_free((void *)&data->line);
	return (SUCCESS);
}

int	is_operator(char c)
{
	return ((c == '|') || (c == '<') || (c == '>'));
}
