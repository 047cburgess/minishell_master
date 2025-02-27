/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:01:22 by caburges          #+#    #+#             */
/*   Updated: 2025/02/13 14:01:42 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_pwd(void)
{
    char  *path; 

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		perror("getcwd");
		return (1);
	}
	printf("%s\n", path);
	free(path);
	return (0);
}

