/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var_export.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:08:40 by alize             #+#    #+#             */
/*   Updated: 2025/03/17 13:54:03 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*find_var_in_list(t_env *list, const char *key)
{
	while (list)
	{
		if (ft_strcmp(list->key, key) == 0)
			return (list);
		list = list->next;
	}
	return (NULL);
}

int	update_or_add_var_env(t_data *data, char *key, char *value)
{
	t_env	*existing_var;
	t_env	*new_node;
	char	*temp_value;

	existing_var = find_var_in_list(data->env, key);
	if (existing_var)
	{
		temp_value = ft_strdup(value);
		if (!temp_value)
			return (ft_dprintf(2, "Malloc failed\n"), 1);
		free(existing_var->value);
		existing_var->value = temp_value;
	}
	else
	{
		new_node = ft_new_node(key, value);
		if (new_node)
			env_add_back(&data->env, new_node);
		else
			return (1);
	}
	env_remove_node(&data->export, key);
	return (0);
}

int	update_or_add_var_export(t_data *data, char *key)
{
	t_env	*new_node;
	char	*key_content;

	if (find_var_in_list(data->export, key) || find_var_in_list(data->env, key))
		return (0);
	key_content = ft_strdup("");
	if (!key_content)
		return (1);
	new_node = ft_new_node(key, key_content);
	if (new_node)
	{
		new_node->no_value = 1;
		env_add_back(&data->export, new_node);
	}
	else
	{
		free(key_content);
		return (1);
	}
	free(key_content);
	return (0);
}

int	add_var_in_export(t_data *data, char *av)
{
	char	*new_key;
	int		status;

	status = 0;
	new_key = ft_strdup(av);
	if (!new_key)
		return (1);
	if (!key_is_valid(new_key))
	{
		ft_dprintf(2, "Minishell: export: « %s » : not a valid identifier\n",
			av);
		free(new_key);
		return (1);
	}
	status = update_or_add_var_export(data, new_key);
	free(new_key);
	return (status);
}

int	add_var_in_env(t_data *data, char *av, char *sign_egal)
{
	char	*new_key;
	char	*key_content;
	int		status;

	status = 0;
	new_key = ft_substr(av, 0, sign_egal - av);
	key_content = ft_strdup(sign_egal + 1);
	if (!new_key || !key_content)
		status = 1;
	if (status == 0 && key_is_valid(new_key))
		status = update_or_add_var_env(data, new_key, key_content);
	else
	{
		ft_dprintf(2, "Minishell: export: « %s » : not a valid identifier\n",
			av);
		status = 1;
	}
	free(new_key);
	free(key_content);
	return (status);
}
