/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:19:45 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/02 18:56:27 by alize            ###   ########.fr       */
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

void	print_ascii_export(t_data *data)
{
	t_env	*current;
	t_env	**export_tab;
	int		size;
	int		i;

	size = env_lst_size(data->env) + env_lst_size(data->export);
	export_tab = malloc(sizeof(t_env*) * size);
	if (!export_tab)
		return ;
	current = data->env;
	i = 0;
	while (current)
	{
		export_tab[i] = current;
		current = current->next;
		i++;
	}
	current = data->export;
	while (current)
	{
		export_tab[i] = current;
		current = current->next;
		i++;
	}
	bubble_sort_ascii(export_tab, size);
	i = 0;
	while (i < size)
	{
		if (export_tab[i]->no_value == 1) 
			printf("declare -x %s", export_tab[i]->key);
		else
			printf("declare -x %s=\"%s\"", export_tab[i]->key, export_tab[i]->value);
		printf("\n");
		i++;
	}
	free(export_tab);
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

void	check_var_export(t_data *data, char *key)
{
	t_env 	*current;
	char	*key_content;
	t_env	*new_node;

	current = data->export;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return ;
		current = current->next;
	}
	key_content = ft_strdup("");
	if (!key_content)
	{
		free(key);
		return ;
	}
	new_node = env_node_new(key, key_content);
	new_node->no_value = 1;
	printf("export node: key = %s, content = %s\n", new_node->key, new_node->value);
	env_add_back(&data->export, new_node);	
}

int var_export(t_data *data, char *av)
{
	char	*new_key;

	new_key = ft_strdup(av);
	if (!new_key)
		return (1);
	if (!key_is_valid(new_key))
	{
		ft_dprintf(2, "Minishell: export: « %s » : not a valid identifier\n", av);
		return (free(new_key), 1);
	}
	check_var_export(data, new_key);
	return (0);
}

int var_env(t_data *data, char *av, char *sign_egal)
{
	char	*new_key;
	char	*key_content;
	int 	len;
	
	len = sign_egal - av;
	new_key = ft_substr(av, 0, len);
	printf("KEY = %s\n", new_key);
	if (!new_key)
		return (1);
	key_content = ft_strdup(sign_egal + 1);
	printf("KEY CONTENT = %s\n", key_content);
	if (!key_content)
		return (free(new_key), 1);
	if (key_is_valid(new_key))
		check_var_env(data, new_key, key_content);
	else
	{
		ft_dprintf(2, "Minishell: export: « %s » : not a valid identifier\n", av);
		return (free(new_key), free(key_content), 1);
	}
	return (0);
}

int	ft_export(char **av, t_data *data)
{
	int		i;
	int		size;
	char	*sign_egal;
	int 	error_status;

	size = count_ac(av);
	if (size == 1)
	{
		print_ascii_export(data);
		return (0);
	}
	i = 1;
	error_status = 0;
	while (av[i])
	{
		if (av[i][0] == '\0' || av[i][0] == '%' || ft_isdigit(av[i][0]))
		{
			ft_dprintf(2, "Minishell: export: « %s » : not a valid identifier\n", av[i]);
			error_status = 1;
		}
		else
		{
			sign_egal = ft_strchr(av[i], '=');
			if (sign_egal == NULL)
			{
				if (var_export(data, av[i]) != 0)
					error_status = 1;
				print_env_list(data->export);
			}
			else
			{
				if (var_env(data, av[i], sign_egal) != 0)
					error_status = 1;
			}
		}
		i++;
	}
	if (error_status == 1)
		data->status = 1;
	else
		data->status = 0;
	return (data->status);
}

