/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 18:12:22 by alize             #+#    #+#             */
/*   Updated: 2025/02/23 22:06:45 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *content_map(char *content)
{
	size_t 	len;
	char 	*new_content;
	
	len = ft_strlen(content);
	new_content = malloc(sizeof(char) * (len + 1));
	if (!new_content)
		return (NULL);
	if (len == 1 && content[0] == '\"')
		content = ft_memset(new_content, '1', len);
	else if (len == 1 && content[0] == '\'')
		content = ft_memset(new_content, '2', len);
	else
		content = ft_memset(new_content, '0', len);
	new_content[len] = '\0';
	return (new_content);
}

t_list	*ft_lst_map(t_list *lst, char *(*f)(char *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;
	char	*new_content;

	new_list = NULL;
	while (lst)
	{
		new_content = f(lst->content);
		new_node = ft_lstnew(new_content);
		if (!new_node)
		{
			(*del)(new_content);
			ft_lstclear(&new_list, (*del));
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_node);
		lst = lst->next;
	}
	return (new_list);
}

void print_map(t_list *map_list)
{
	t_list *current;

	current = map_list;
	printf("--MAPPING--\n");
	while (current)
	{
		printf("[%s]\n", (char *)current->content);
		current = current->next;
	}
}

int ft_mapping(t_data *data, t_list *cutting)
{
	data->map_list = ft_lst_map(cutting, content_map, free);
	if (!data->map_list)
		return (FAILURE);
	print_map(data->map_list);
	return (SUCCESS);
}

void delete_quotes(t_list **cutting)
{
	t_list *current;
	t_list *prev;
	t_list *delete_node;

	current = *cutting;
	prev = NULL;
	while (current)
	{
		if (ft_strlen(current->content) == 1
			&& ((*(char *)current->content == '\'' || (*(char *)current->content == '\"'))))
		{
			delete_node = current;
			current = current->next;
			if (prev)
				prev->next = current->next;
			else
				*cutting = current->next,
			free(delete_node->content);
			free(delete_node);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}