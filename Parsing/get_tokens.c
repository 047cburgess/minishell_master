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

t_token	*get_quoted_token(char *start, char quote)
{
	t_token	*new_token = NULL;
	char	*new_content = NULL;
	char	q;
	int i = 0;
	(void)quote;

	while(1)
	{
		if (is_quote(start[i]))
		{
			q = start[i];
			i++;
			while (start[i] != q)
				i++;
			i++;
		}
		if (start[i] == '\0' || start[i] == ' ' || is_operator(start[i]))
			break;
		else
			while (start[i] && !is_quote(start[i]) && start[i] != ' ' && !is_operator(start[i]))
				i++;
	}
	new_content = ft_substr(start, 0, i);
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
	char	*ptr = start;
	int len;
	t_token	*new_token = NULL;
	char	*new_content = NULL;
	int i = 0;

	while (*ptr && !is_operator(*ptr) && *ptr != ' ')
	{
		if (is_quote(*ptr))
		{
			ptr = ft_strchr(ptr + 1, *ptr) + 1;
		}
		else 
			ptr++;
		i++;
	}
	len = ptr - start;
	new_content = ft_substr(start, 0, len);
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

t_token	*get_operator_token(char *start)
{
	char	*content;
	t_token *token;
	int	type;

	content = ft_calloc(MAX_OPERATOR_LEN + 1, sizeof(char));
	if (!content)
		return (NULL);
	if (ft_strncmp(start, "<<", 2) == 0)
	{
		ft_strcpy(content,"<<");
		type = RD_HEREDOC;
	}
	else if (ft_strncmp(start, ">>", 2) == 0)
	{
		ft_strcpy(content,">>");
		type = RD_APPEND;
	}
	else if (*start == '|')
	{
		ft_strcpy(content,"|");
		type = PIPE;
	}
	else if (*start == '<')
	{
		ft_strcpy(content,"<");
		type = RD_IN;
	}
	else
	{
		ft_strcpy(content,">");
		type = RD_OUT;
	}
	token = new_token_node(content);
	if (!token)
	{
		free(content);
		return (NULL);
	}
	token->type = type;
	return (token);
}

// WILL NEED TO ADAPT THIS FUNCTION ACCORDING TO NEW CUTTING/EXPANSION RULES
int	tokenise(char *line, t_data *data)
{
	data->tokens_list = NULL;
	t_token	*new_token = NULL;

	int i = 0;
	while (line[i] != '\0')
	{
		while (isspace(line[i]))
			i++;
		if (is_operator(line[i]))
		{
			new_token = get_operator_token(&line[i]);
			if (!new_token)
				return (token_lst_clear(&data->tokens_list, free), FAILURE);
			token_add_back(&(data->tokens_list), new_token);
			i += ft_strlen(new_token->content);
		}
		else if (is_quote(line[i]))
		{
			new_token = get_quoted_token(&line[i], line[i]);
			if (!new_token)
				return (token_lst_clear(&data->tokens_list, free), FAILURE);
			token_add_back(&(data->tokens_list), new_token);
			i += ft_strlen(new_token->content);
		}
		else if (!is_quote(line[i]) && line[i] != '\0')
		{
			new_token = get_unquoted_token(&line[i]);
			if (!new_token)
				return (token_lst_clear(&data->tokens_list, free), FAILURE);
			token_add_back(&(data->tokens_list), new_token);
			i += ft_strlen(new_token->content);
		}
	}
	ft_dprintf(data->log, "--TOKENS--\n\t");
	print_tokens_list(data->log, data->tokens_list);
	return (SUCCESS);
}
