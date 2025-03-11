/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:23:01 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/04 15:25:14 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_remove_node(t_env **list, char *key)
{
	t_env	*current;
	t_env	*temp;
	t_env	*prev;

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
			break; // added BREAK
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(char **av, t_data *data)
{
	int	i;

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
