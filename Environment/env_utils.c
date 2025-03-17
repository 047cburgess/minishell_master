/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:56:46 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/17 13:59:28 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// This function creates and returns a new node according to given key and value
t_env	*env_node_new(char *key, char *value)
{
	t_env	*new_node;

	if (!key || !value)
		return (NULL);
	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = key;
	new_node->value = value;
	new_node->no_value = 0;
	new_node->next = NULL;
	return (new_node);
}

// This function deletes a node from the environment list
void	env_delone(t_env *node)
{
	free(node->key);
	free(node->value);
	node->key = NULL;
	node->value = NULL;
	node->no_value = 0;
	free(node);
}

// This function deletes / frees the whole env list
void	env_list_clear(t_env **env_list)
{
	t_env	*temp;

	while (*env_list)
	{
		temp = (*env_list)->next;
		env_delone(*env_list);
		*env_list = temp;
	}
	*env_list = NULL;
}

// This function returns the last element of the environment list
t_env	*env_last(t_env *env_head)
{
	if (!env_head)
		return (NULL);
	while (env_head->next != NULL)
		env_head = env_head->next;
	return (env_head);
}

// This function adds a new node (already created) to the back of the env list
void	env_add_back(t_env **env_head, t_env *new_node)
{
	t_env	*last;

	if (*env_head == NULL)
		*env_head = new_node;
	else
	{
		last = env_last(*env_head);
		last->next = new_node;
	}
}
