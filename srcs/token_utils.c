/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:31:36 by alize             #+#    #+#             */
/*   Updated: 2025/02/19 19:10:16 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *new_token_node(char *content)
{
	t_token *new;
	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->content = content;
	new->type = 0;
	new->next = NULL;
	return (new);
}

void	token_add_back(t_token **tokens, t_token *new)
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

void	token_del_node(t_token *tokens_list, void (*del)(void *))
{
	(*del)(tokens_list->content);
	free(tokens_list);
}

void	token_lst_clear(t_token **tokens_list, void (*del)(void *))
{
	t_token	*temp;

	while (*tokens_list)
	{
		temp = (*tokens_list)->next;
		token_del_node(*tokens_list, (*del));
		*tokens_list = temp;
	}
	*tokens_list = NULL;
}

t_token	*token_lst_last(t_token *head)
{
	if (!head)
		return (NULL);
	while (head->next != NULL)
		head = head->next;
	return (head);
}

void print_tokens_list(t_data *data)
{
	t_token *current;
	
	current = data->tokens_list;
	while (current)
	{
		printf("[%s]->", current->content);
		current = current->next;
	}
	printf("\n");
}

int	is_operator(char c)
{
	return ((c == '|') || (c == '<') || (c == '>'));
}
