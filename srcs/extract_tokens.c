/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:50:37 by alsuchon          #+#    #+#             */
/*   Updated: 2025/02/17 18:46:57 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token *new_token(char *content)
{
	t_token *new;
	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->content = ft_strdup(content);
	new->type = 0;
	new->next = NULL;
	return (new);
}

static void	token_add_back(t_token **tokens, t_token *new)
{
	t_token	*current;

	if (!tokens || !new)
		return ;
	if (*tokens == NULL)
	{
		*tokens = new;
		return ;
	}
	current = *tokens;
	while (current->next)
	{
		current = current->next;
	}
	current->next = new;
}

static int is_space(char c)
{
	return (c == ' ');
}

static char *handle_quote(char *line)
{
	char *word;
	char quote;
	int len;
	int i;

	
	len = 0;
	word = NULL;
	quote = *line;
	i = 1;
	while (line[i] && line[i] != quote)
	{
		len++;
		i++;
	}
	if (*line == quote)
		word = ft_substr(line, 1, len);
	return (word);
}

static char *extract_word(char *line)
{
	int len;
	int i;
	char *word;

	len = 0;
	i = 0;
	word = NULL;
	while (line[i] && !is_space(line[i]))
	{	
		len++;
		i++;
	}
	word = ft_substr(line, 0, len);
	return (word);
}

t_token *extract_tokens(char *line)
{
	t_token *head = NULL;
	t_token	*new;
	char *str;
	int i;
	
	i = 0;
	str = NULL;
	new = NULL;
	if (unclosed_quote_detected(line) == TRUE)
		return (ft_putstr_fd("No quotes closed\n", 2), NULL);
	while (*line)
	{
		while (is_space(*line))
			line++;
		if (is_quote(*line))
		{
			str = handle_quote(line);
			new = new_token(str);
			token_add_back(&head, new);
			line = (ft_strchr(line + 1, *line)) + 1;
		}	
		else if (*line != '\0')
		{
			str = extract_word(line);
			new = new_token(str);
			token_add_back(&head, new);
			line += ft_strlen(new->content);
		}
	}
	return (head);
}

