/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 12:29:45 by caburges          #+#    #+#             */
/*   Updated: 2025/03/12 10:55:24 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"
#include <stdarg.h>

t_list	*ft_printformat(t_list **head, va_list args, char form, t_printf *data)
{
	char	*return_string;
	t_list	*new_node;

	return_string = NULL;
	if (form == 'i' || form == 'd')
		return_string = ft_printnbr(va_arg(args, int));
	else if (form == 's')
		return_string = ft_printstr(va_arg(args, char *));
	if (!return_string)
	{
		return (NULL);
	}
	new_node = ft_lstnew(return_string);
	if (!new_node)
	{
		free(return_string);
		data->error = 1;
		return (NULL);
	}
	ft_lstadd_back(head, new_node);
	return (new_node);
}

t_list	*extract_until_next(t_list **head, const char **ptr, t_printf *data)
{
	char		*return_string;
	const char	*str;
	t_list		*new_node;
	int			i;

	str = *ptr;
	i = 0;
	if (str[i] == '%')
		i++;
	while (str[i] && str[i] != '%')
		i++;
	return_string = ft_substr(str, 0, i);
	if (!return_string)
	{
		data->error = 1;
		return (NULL);
	}
	new_node = ft_lstnew(return_string);
	if (!new_node)
	{
		data->error = 1;
		return (free(return_string), NULL);
	}
	*ptr = *ptr + i;
	return (ft_lstadd_back(head, new_node), new_node);
}

int	get_final_len(t_list *head)
{
	int	i;

	i = 0;
	while (head)
	{
		i += ft_strlen(head->content);
		head = head->next;
	}
	return (i);
}

char	*set_final_line(char **line, t_list *head, int error)
{
	int		len;
	char	*final_line;

	if (!head || *line || error)
		return (NULL);
	len = get_final_len(head);
	final_line = ft_calloc(len + 1, sizeof(char));
	if (!final_line)
	{
		*line = NULL;
		return (NULL);
	}
	while (head)
	{
		ft_strlcat(final_line, head->content, len + 1);
		head = head->next;
	}
	*line = final_line;
	return (final_line);
}

int	ft_dprintf(int fd, const char *str, ...)
{
	va_list		args;
	t_printf	data;

	if (str == NULL || fd < 0)
		return (-1);
	init_printf(&data);
	va_start(args, str);
	while (*str != '\0')
	{
		if (*str == '%' && ft_strchr("ids", *(str + 1)))
		{
			if (!ft_printformat(&data.cutting, args, *(str + 1), &data))
				break ;
			str += 2;
		}
		else if (!extract_until_next(&data.cutting, &str, &data))
			break ;
	}
	va_end(args);
	if (set_final_line(&data.final_line, data.cutting, data.error))
		data.line_len = write(fd, data.final_line, ft_strlen(data.final_line));
	ft_lstclear(&data.cutting, free);
	ft_free((void *)&data.final_line);
	return (data.line_len);
}
