/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:23:01 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/03 16:33:18 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void env_remove_node(t_env **list, char *key)
{
	t_env 	*current = NULL;
	t_env 	*temp;
	t_env 	*prev;

	if (!list || !*list || !key)
		return ;
	current = *list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			temp = current;
			if (prev == NULL)
				*list = current->next;
			else
				prev->next = current->next;
			env_delone(temp);
		}
		prev = current;
		current = current->next;
	}
}

int ft_unset(char **av, t_data *data)
{
	int i;

	if (!av[1])
		return (0);
	i = 1;
	while (av[i])
	{
		env_remove_node(&(data->env), av[i]);
		env_remove_node(&(data->export), av[i]);
		i++;
	}
	return (0);
}
