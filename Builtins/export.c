/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:19:45 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/05 11:20:57 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_export_display(t_data *data, int size)
{
	if (size == 1)
	{
		print_ascii_export(data);
		return (1);
	}
	return (0);
}

int	handle_invalid_identifier(char *av)
{
	if (av[0] == '\0' || ft_isdigit(av[0]))
	{
		ft_dprintf(2, "Minishell: export: « %s » : not a valid identifier\n",
			av);
		return (1);
	}
	return (0);
}

int	process_export_argument(t_data *data, char *av)
{
	char	*sign_egal;

	sign_egal = ft_strchr(av, '=');
	if (sign_egal == NULL)
		return (add_var_in_export(data, av));
	else
		return (add_var_in_env(data, av, sign_egal));
}

int	ft_export(char **av, t_data *data)
{
	int	i;
	int	error_status;
	int	size;

	i = 1;
	error_status = 0;
	size = count_ac(av);
	if (handle_export_display(data, size))
		return (0);
	while (av[i])
	{
		if (handle_invalid_identifier(av[i]))
			error_status = 1;
		else if (process_export_argument(data, av[i]) != 0)
			error_status = 1;
		i++;
	}
	data->status = error_status;
	return (data->status);
}
