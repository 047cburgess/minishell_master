/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:23:01 by alsuchon          #+#    #+#             */
/*   Updated: 2025/02/28 17:37:38 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_unset(char **av, t_data *data)
{
	int 	i;
	t_env	*current_env;
	t_env 	*temp;
	t_env 	*prev;

	if (!av[1])
		return (0);
	i = 1;
	while (av[i])
	{
		current_env = data->env;
		prev = NULL;
		while (current_env)
		{
			if (ft_strcmp(current_env->key, av[i]) == 0)
			{
				temp = current_env;
				if (prev == NULL)
					data->env = current_env->next;
				else
					prev->next = current_env->next;
				env_delone(temp);
				break;
			}
			prev = current_env;
			current_env = current_env->next;
		}
		i++;
	}
	return (0);
}