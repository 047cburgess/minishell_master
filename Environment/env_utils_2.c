/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:58:38 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/17 13:59:53 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// This function prints the env list like bash
void	print_env_list(t_env *env_head)
{
	while (env_head != NULL)
	{
		printf("%s=%s\n", env_head->key, env_head->value);
		env_head = env_head->next;
	}
}

// This function returns the list size of minishell env
int	env_lst_size(t_env *env)
{
	int	size;

	size = 0;
	while (env != NULL)
	{
		size++;
		env = env->next;
	}
	return (size);
}
