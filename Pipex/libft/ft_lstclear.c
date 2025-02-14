/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:21:07 by caburges          #+#    #+#             */
/*   Updated: 2024/11/19 15:54:13 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp_lst;

	while (*lst)
	{
		temp_lst = (*lst)->next;
		ft_lstdelone(*lst, (*del));
		*lst = temp_lst;
	}
	*lst = NULL;
}
