/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:01:24 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/03 16:08:00 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	key_is_valid(char *key)
{
	int	i;

	i = 0;
	if (!key)
		return (FALSE);
	if (key[0] == '\0' || key[0] == '=')
		return (FALSE);
	if (ft_isdigit(key[0]))
		return (FALSE);
	while (key[i] && key[i] != '=')
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

t_env *search_key(t_env *list, char *key)
{
	while (list)
	{
		if (ft_strcmp(list->key, key) == 0)
			return (list);
		list = list->next;
	}
	return (NULL);
}

void	bubble_sort_ascii(t_env **env_tab, int size)
{
	int		sorted;
	int		i;
	t_env	*temp;

	sorted = 1;
	while (sorted)
	{
		sorted = 0;
		i = 0;
		while (i < (size -1))
		{
			if (ft_strcmp(env_tab[i]->key, env_tab[i + 1]->key) > 0)
			{
				temp = env_tab[i];
				env_tab[i] = env_tab[i + 1];
				env_tab[i + 1] = temp;
				sorted = 1;
			}
			i++;
		}
	}
}
