/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:56:19 by caburges          #+#    #+#             */
/*   Updated: 2025/03/07 14:28:58 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void update_pwd_value(t_data *data, char *key, char *value)
{
	t_env *existing_var;

	existing_var = find_var_in_list(data->env, key);
	if (existing_var)
	{
		free(existing_var->value);
		if (!value)
			existing_var->value = ft_strdup("");
		else
			existing_var->value = ft_strdup(value);
		if (!existing_var->value)
			env_remove_node(&data->env, key);
	}
}

int	ft_cd(char **av, t_data *data)
{
	char	*new_pwd;
	int	ac;

	ac = count_ac(av);
	if (ac > 1)
	{
		ft_dprintf(2, "Minishell: cd: too many arguments\n");
		return (1);
	}
	else if (ac == 0)
	{
		ft_dprintf(2, "Minishell: cd: Not enough argument\n");
		return (1);
	}
	else if (chdir(av[0]) == -1)
	{
		ft_dprintf(2, "Minishell: cd: %s: %s\n", av[0], strerror(errno));
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	update_pwd_value(data, "OLDPWD", ft_getenv(data->env, "PWD"));
	update_pwd_value(data, "PWD", new_pwd);
	ft_free((void *)&new_pwd);
	return (0);
}
