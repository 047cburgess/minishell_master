/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:19:45 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/03 17:51:39 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_ascii_export(t_data *data)
{
	t_env	*current;
	t_env	**export_tab;
	int		size;
	int		i;

	size = env_lst_size(data->env) + env_lst_size(data->export);
	export_tab = malloc(sizeof(t_env*) * (size + 1));
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

void handle_assignememt_var(t_data *data, char *key, char *value, int with_egal)
{
	t_env *env_node;
	t_env *export_node;
	t_env *new_node;
	t_env *move_node;

	env_node = search_key(data->env, key);
	export_node = search_key(data->export, key);

	if (with_egal)
	{
		if (env_node) //si existe dans env/ mise a jour
		{
			free(env_node->value);
			env_node->value = ft_strdup(value);
		}
		else if (export_node) // Si existe dans export, on le déplace vers env
        {
            move_node = search_key(data->export, key);
            if (move_node)
            {
                env_remove_node(&(data->export), key);
                move_node->value = ft_strdup(value);
                move_node->no_value = 0;
                env_add_back(&(data->env), move_node);
            }
        }
		else //si n'existe pas -> ajout dns env
			env_add_back(&(data->env), env_node_new(key, value));
	}
	else //pas de egal ajout dans export 
	{
		if (!env_node && !export_node)
		{
			new_node = env_node_new(key, "");
			env_add_back(&(data->export), new_node);
			new_node->no_value = 1;
		}
	}
}

int var_export(t_data *data, char *av)
{
	char	*new_key;
	t_env 	*export_node;

	new_key = ft_strdup(av);
	if (!new_key)
		return (1);
	if (!key_is_valid(new_key))
	{
		ft_dprintf(2, "Minishell: export: « %s » : not a valid identifier\n", av);
		return (free(new_key), 1);
	}
	export_node = search_key(data->export, new_key);
	if (!export_node)
		handle_assignememt_var(data, new_key, NULL, 0);
	free(new_key);
	return (0);
}

int var_env(t_data *data, char *av, char *sign_egal)
{
	char	*new_key;
	char	*key_content;
	int 	len;
	
	len = sign_egal - av;
	new_key = ft_substr(av, 0, len);
	if (!new_key)
		return (1);
	key_content = ft_strdup(sign_egal + 1);
	if (!key_content)
		return (free(new_key), 1);
	if (key_is_valid(new_key))
		handle_assignememt_var(data, new_key, key_content, 1);
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
			}
			else
			{
				if (var_env(data, av[i], sign_egal) != 0)
					error_status = 1;
			}
		}
		i++;
	}
	data->status = error_status;
	return (data->status);
}

