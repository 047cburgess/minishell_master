/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 12:48:16 by alize             #+#    #+#             */
/*   Updated: 2025/02/24 23:41:16 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env_list_test(t_env *env_head);
void remove_quotes(char *variable_content); // to do : fonction pour retirer les quotes

bool	key_is_valid(char *key)
{
	int	i;
	
	if (!key || ft_isdigit(key[0]))
		return (FALSE);
	i = 1;
	while (key[i] && key[i] != '=')
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

void check_var_env(t_data *data, char *key, char *key_content)
{
	t_env	*env;
	t_env	*new_node;
	
	env = data->env;
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			free(env->value);
			env->value = ft_strdup(key_content);
			return ;
		}
		env = env->next;
	}
	new_node = env_node_new(key, key_content);
	env_add_back(&(data->env), new_node);
}

void	ft_export(char **av, t_data *data)
{
	t_env	*env;
	char 	*new_key;
	char	*key_content;
	char	*sign_egal;
	int 	i;

	env = data->env;
	if (!av[1])
	{
		while (env)
		{
			printf("%s=%s\n", env->key, env->value);
			env = env->next;
		}
		return ;
	}
	i = 1;
	while (av[i])
	{
		sign_egal = ft_strchr(av[i], '=');
		if (sign_egal == NULL)
			i++; //SI PAS DE "=" ON IGNORE ET ON SAUTE À L'ARGUMENT SUIVANT
		new_key = ft_substr(av[i], 0, sign_egal - av[i]);
		if (*(sign_egal + 1) == '\0')
			key_content = ft_strdup("");
		else
			key_content = ft_strdup(sign_egal + 1);
		if (key_is_valid(new_key))
		{
			check_var_env(data, new_key, key_content);
		}
		else
			printf("Minishell: export: « %s » : not a valid identifier\n", av[i]);
		i++;
	}
}