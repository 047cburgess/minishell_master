/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:19:45 by alsuchon          #+#    #+#             */
/*   Updated: 2025/02/27 16:46:41 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	print_ascii_env(t_env *env)
{
	t_env	*current;
	t_env	**env_tab;
	int		size;
	int		i;

	size = env_lst_size(env);
	env_tab = malloc(sizeof(t_env*) * size);
	if (!env_tab)
		return ;
	current = env;
	i = 0;
	while (i < size)
	{
		env_tab[i] = current;
		current = current->next;
		i++;
	}
	bubble_sort_ascii(env_tab, size);
	i = 0;
	while (i < size)
	{
		printf("declare -x %s", env_tab[i]->key);
		if (env_tab[i]->value)
			printf("=\"%s\"", env_tab[i]->value);
		printf("\n");
		i++;
	}
	free(env_tab);
}

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

void	check_var_env(t_data *data, char *key, char *key_content)
{
	t_env	*env;
	t_env	*new_node;

	env = data->env;
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			free(env->value);
			if (key_content)
				env->value = ft_strdup(key_content);
			else
				env->value = NULL;
			return ;
		}
		env = env->next;
	}
	new_node = env_node_new(key, key_content);
	env_add_back(&(data->env), new_node);
}

int	ft_export(char **av, t_data *data)
{
	char	*new_key;
	char	*key_content;
	int		i;
	int		size;
	char	*sign_egal;

	size = count_ac(av);
	if (size == 1)
	{
		print_ascii_env(data->env);
		return (0);
	}
	i = 1;
	while (av[i])
	{
		if (av[i][0] == '\0')
		{
			printf("Minishell: export: « %s » : not a valid identifier\n", av[i]);
			return (1);
		}
		sign_egal = ft_strchr(av[i], '=');
		if (sign_egal == NULL)
		{
			if (size > 2)
			{
				i++;
				continue ;
			}
			else
				return (0);
		}
		new_key = ft_substr(av[i], 0, sign_egal - av[i]);
		if (*(sign_egal + 1) == '\0')
			key_content = ft_strdup("");
		else
			key_content = ft_strdup(sign_egal + 1);
		if (key_is_valid(new_key))
			check_var_env(data, new_key, key_content);
		else
		{
			printf("Minishell: export: « %s » : not a valid identifier\n", av[i]);
			return (1);
		}
		i++;
	}
	return (0);
}
