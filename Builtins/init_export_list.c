/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_export_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:30:58 by alize             #+#    #+#             */
/*   Updated: 2025/03/11 18:26:02 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_new_node(char *key)
{
	t_env	*new_node;

	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (!new_node->key)
	{
		free(new_node);
		return (NULL);
	}
	return (new_node);
}

t_env	*ft_new_node(char *key, char *value)
{
	t_env	*new_node;

	if (!key)
		return (NULL);
	new_node = init_new_node(key);
	if (!new_node)
		return (NULL);
	if (value)
	{
		new_node->value = ft_strdup(value);
		if (!new_node->value)
		{
			free(new_node->key);
			//free(new_node->value);
			free(new_node);
			//env_delone(new_node);
			return (NULL);
		}
	}
	new_node->no_value = (value == NULL);
	return (new_node);
}
