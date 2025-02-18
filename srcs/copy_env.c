/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:01:26 by caburges          #+#    #+#             */
/*   Updated: 2025/02/17 17:28:07 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// This function prints the env list like bash
void	print_env_list_test(t_env *env_head)
{
	while (env_head != NULL)
	{
		printf("%s=%s\n", env_head->key, env_head->value);
		env_head = env_head->next;
	}
}

// This function creates and returns a new node according to given key and value
t_env	*env_node_new(char *key, char *value)
{
	t_env	*new_node;

	if (!key || !value)
		return (NULL);
	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	return (new_node);
}

// This function extracts a the key from the environment string e.g. "HOME=/home/caburges/" returns "HOME"
char	*get_key(char *str)
{
	char	*key;
	int	key_len;

	key_len = 0;
	while (str[key_len] != '=')
		key_len++;
	key = ft_substr(str, 0, key_len);
	return (key);
}

// This function extracts the value from the environment string
char	*get_value(char *str)
{
	char	*value;
	char	*value_start;

	value_start = ft_strchr(str, '=') + 1;
	value = ft_strdup(value_start);
	return (value);
}

// This function deletes a node from the environment list (does not connect the list back together)
void	env_delone(t_env *node)
{
	free(node->key);
	free(node->value);
	free(node);
}

// This function deletes / frees the whole env list
void	env_list_clear(t_env **env_list)
{
	t_env	*temp;

	while (*env_list)
	{
		temp = (*env_list)->next;
		env_delone(*env_list);
		*env_list = temp;
	}
	*env_list = NULL;
}

// This function returns the last element of the environment list
t_env	*env_last(t_env *env_head)
{
	if (!env_head)
		return (NULL);
	while (env_head->next != NULL)
		env_head = env_head->next;
	return (env_head);
}

// This function adds a new node (already created) to the back of the env list
void	env_add_back(t_env **env_head, t_env *new_node)
{
	t_env	*last;

	if (*env_head == NULL)
		*env_head = new_node;
	else
	{
		last = env_last(*env_head);
		last->next = new_node;
	}
}

// This function converts bash env to a list structure and returns it
t_env	*env_to_list(char **bash_env)
{
	t_env	*minishell_env;
	t_env	*new_node;
	char	*key;
	char	*value;

	minishell_env = NULL;
	new_node = NULL;
	key = NULL;
	value = NULL;
	while (*bash_env != NULL)
	{
		key = get_key(*bash_env);
		value = get_value(*bash_env);
		new_node = env_node_new(key, value);
		if (!new_node)
		{
			free(key);
			free(value);
			env_list_clear(&minishell_env);

		}
		env_add_back(&minishell_env, new_node);
		bash_env++;
	}
	return (minishell_env);
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

// This function converts minishell env list back to an array
char	**env_to_array(t_env *env_head)
{
	int	list_size;
	int	strings_len;
	char	**array;
	int	i;

	list_size = env_lst_size(env_head);
	array = ft_calloc(list_size + 1, sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	while (i < list_size)
	{
		strings_len = ft_strlen(env_head->key) + ft_strlen(env_head->value);
		array[i] = ft_calloc(strings_len + 2, sizeof(char));
		if (!array[i])
		{
			return (free_str_array(array, i), NULL);
		}
		ft_strcpy(array[i], env_head->key);
		ft_strlcat(array[i], "=", strings_len + 2);
		ft_strlcat(array[i], env_head->value, strings_len + 2);
		i++;
		env_head = env_head->next;
	}
	return (array);
}

// This function copies bash env at minishell startup and puts it into a linked list
int	set_environment(char **bash_env, t_data *data)
{
	ft_bzero(data, sizeof(data));
	data->bash_env = bash_env;
	data->env = env_to_list(bash_env);
	if (data->env == NULL)
	{
		ft_putendl_fd("Fatal: failed to initialise startup env", 2);
		return (FAILURE);
	}
	else
		return (SUCCESS);
}

// This function returns a pointer to the env value, or NULL if it doesn't exist
char	*ft_getenv(t_env *env, char *key)
{
	while (env != NULL)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			return (env->value);

		}
		env = env->next;
	}
	return (NULL);
}
