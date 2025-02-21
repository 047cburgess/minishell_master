/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:30:22 by alize             #+#    #+#             */
/*   Updated: 2025/02/21 11:13:19 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*get_quoted_token(char *start, char quote)
{
	t_token	*new_token = NULL;
	char	*new_content = NULL;
	int i = 1;

	while (start[i] != quote)
		i++;
	while (start[i] && start[i] != ' ' && !is_operator(start[i]))
		i++;
	new_content = ft_substr(start, 0, i);
	new_token = new_token_node(new_content);
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
	new_token = new_token_node(new_content);
	return (new_token);
}

t_token	*get_operator(char *start)
{
	char	*content;
	t_token *token;
	
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
	token = new_token_node(content);
	return (token);
}

// WILL NEED TO ADAPT THIS FUNCTION ACCORDING TO NEW CUTTING/EXPANSION RULES
int	parse_and_execute(char *line, t_data *data)
{
	data->tokens_list = NULL;
	t_token	*new_token = NULL;

	if (unclosed_quote_detected(line))
		return (FAILURE);
	printf("--TOKENS--\n");
	// GET TOKEN LIST
	int i = 0;
	while (line[i] != '\0')
	{
		while (isspace(line[i]))
			i++;
		if (is_operator(line[i]))
		{
			new_token = get_operator(&line[i]);
			printf("\t[%s]\n", (char *)new_token->content);
			token_add_back(&(data->tokens_list), new_token);
			i += ft_strlen(new_token->content);
		}
		else if (is_quote(line[i]))
		{
			new_token = get_quoted_token(&line[i], line[i]);
			token_add_back(&(data->tokens_list), new_token);
			printf("\t[%s]\n", (char *)new_token->content);
			i += ft_strlen(new_token->content);
		}
		else if (!is_quote(line[i]))
		{
			new_token = get_unquoted_token(&line[i]);
			token_add_back(&(data->tokens_list), new_token);
			printf("\t[%s]\n", (char *)new_token->content);
			i += ft_strlen(new_token->content);
		}
	}
	return (SUCCESS);
}
