/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:01:26 by caburges          #+#    #+#             */
/*   Updated: 2025/03/05 13:09:37 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			return (NULL);
		}
		env_add_back(&minishell_env, new_node);
		bash_env++;
	}
	return (minishell_env);
}

// This function copies bash env at minishell startup and puts it into a linked list
int	set_environment(char **bash_env, t_data *data)
{
	t_env *new_node;

	data->bash_env = bash_env;
	if (!bash_env || !*bash_env)
	{
		data->env = NULL;
		new_node = ft_new_node("PATH", "/bin:/usr/bin");
		if (new_node)
			env_add_back(&data->env, new_node);
		else
		{
			ft_putendl_fd("Fatal: failed to allocate memory", 2);
			return (FAILURE);
		}
		return (SUCCESS);
	}
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
