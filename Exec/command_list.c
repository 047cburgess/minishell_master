/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:57:01 by caburges          #+#    #+#             */
/*   Updated: 2025/03/12 10:57:03 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// This function builds a command table, receiving pointer to table, 
t_command	*new_command_table(t_token *tokens)
{
	t_command	*node;

	node = ft_calloc(1, sizeof(t_command));
	if (!node)
	{
		return (NULL);
	}
	node->tokens = tokens;
	node->ac = get_ac(tokens);
	node->av = get_av(tokens, node->ac);
	if (!node->av)
	{
		free(node);
		return (NULL);
	}
	node->fds[0] = -1;
	node->fds[1] = -1;
	node->pid = -1;
	node->next = NULL;
	node->error = 0;
	return (node);
}

void	command_add_back(t_command **head, t_command *new)
{
	t_command	*current;

	if (!head || !new)
		return ;
	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	current = *head;
	while (current->next)
	{
		current = current->next;
	}
	current->next = new;
}

void	command_del_node(t_command *cmd)
{
	free(cmd->av);
	cmd->av = NULL;
	cmd->tokens = NULL;
	free(cmd);
}

void	command_lst_clear(t_command **head)
{
	t_command	*temp;

	while (*head)
	{
		temp = (*head)->next;
		command_del_node(*head);
		*head = temp;
	}
	*head = NULL;
}

t_command	*command_lst_last(t_command *head)
{
	if (!head)
		return (NULL);
	while (head->next != NULL)
		head = head->next;
	return (head);
}
