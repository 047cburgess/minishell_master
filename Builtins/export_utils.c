/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:52:32 by alize             #+#    #+#             */
/*   Updated: 2025/03/04 15:46:45 by alize            ###   ########.fr       */
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

t_env	**create_sorted_export_list(t_data *data, int *size)
{
	t_env	*current;
	t_env	**export_tab;
	int		i;

	i = 0;
	*size = env_lst_size(data->env) + env_lst_size(data->export);
	export_tab = malloc(sizeof(t_env*) * (*size));
	if (!export_tab)
		return (NULL);
	current = data->env;
	while (current)
	{
		export_tab[i++] = current;
		current = current->next;
	}
	current = data->export;
	while (current)
	{
		export_tab[i++] = current;
		current = current->next;
	}
	bubble_sort_ascii(export_tab, *size);
	return (export_tab);
}

void	print_sorted_export_list(t_env **export_tab, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (export_tab[i]->no_value == 1)
			printf("declare -x %s", export_tab[i]->key);
		else
			printf("declare -x %s=\"%s\"",
				export_tab[i]->key, export_tab[i]->value);
		printf("\n");
		i++;
	}
}

int	print_ascii_export(t_data *data)
{
	t_env	**export_tab;
	int		size;

	export_tab = create_sorted_export_list(data, &size);
	if (!export_tab)
		return (1) ;
	print_sorted_export_list(export_tab, size);
	free(export_tab);
	return (0);
}

