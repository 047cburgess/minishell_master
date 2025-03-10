/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_cutting.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:04:47 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/10 14:32:56 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_dollar_alone(t_list **cutting, int *i)
{
	t_list	*new_node;

	new_node = ft_lstnew(ft_strdup("$"));
	if (!new_node)
	{
		ft_lstclear(cutting, free);
		return ;
	}
	ft_lstadd_back(cutting, ft_lstnew(ft_strdup("$")));
	(*i)++;
}

void	handle_exit_extansion(t_data *data, t_list **cutting, char *line, int *i)
{
	char	*expansion;
	char	*result;
	t_list	*new_node;

	if (line[*i] == '$' && line[*i + 1] == '?')
	{
		(*i) += 2;
		result = ft_itoa(data->status);
		if (!result)
			return ;
		expansion = ft_strdup(result);
		free(result);
		new_node = ft_lstnew(expansion);
		ft_lstadd_back(cutting, new_node);
	}
}

t_list	*convert_var_expansion(t_data *data, char *line, int *i)
{
	char	*expansion;
	char	*var_content;
	char	*key;
	int		start;

	start = *i;
	key = find_key(line, *i + 1);
	if (!key)
	return (NULL);
	if (key_is_valid(key) == false)
	{
		*i += ft_strlen(key) + 1;
		expansion = ft_substr(line, start, *i - start);
		free(key);
		return (ft_lstnew(expansion));
	}
	var_content = ft_getenv(data->env, key);
	if (var_content)
		expansion = ft_strdup(var_content);
	else
		expansion = ft_strdup("");
	*i += ft_strlen(key) + 1;
	free(key);
	return (ft_lstnew(expansion));
}

// void	handle_simple_quotes(t_list **cutting, char *line, int *i)
// {
// 	int		start;
// 	char	*new_line;
// 	t_list	*new_node;

// 	(*i)++;
// 	start = *i;
// 	while (line[*i] && line[*i] != '\'')
// 		(*i)++;
// 	if (line[*i] == '\'')
// 	{
// 		new_line = ft_substr(line, start, *i - start);
// 		if (!new_line)
// 			return ;
// 		new_node = ft_lstnew(new_line);
// 		if (!new_node)
// 		{
// 			free(new_line);
// 			return ;
// 		}
// 		ft_lstadd_back(cutting, new_node);
// 		(*i)++;
// 	}
// }

void handle_simple_quotes(t_list **cutting, char *line, int *i)
{
	int start;
	char *new_line;
	t_list *new_node;

	(*i)++;
	start = *i;
	while (line[*i] && line[*i] != '\'')
		(*i)++;
	if (line[*i] == '\'')
	{
		new_line = ft_substr(line, start, *i - start);
		if (!new_line)
		return ;
		//if (new_line[0] == '\0') // Ignore les quotes vides !
		//{
		//	free(new_line);
		//	(*i)++;
		//	return ;
		//}
		new_node = ft_lstnew(new_line);
		ft_lstadd_back(cutting, new_node);
		(*i)++;
	}
}


void	handle_simple_text(t_list **cutting, char *line, int *i)
{
	char	*new_line;
	t_list	*new_node;
	int		start;

	start = *i;
	while (line[*i] && line[*i] != '$' && line[*i] != '\"' && line[*i] != '\'')
	{
		(*i)++;
	}
	new_line = ft_substr(line, start, *i - start);
	if (!new_line)
		return ;
	new_node = ft_lstnew(new_line);
	if (!new_node)
	{
		free(new_line);
		return ;
	}
	ft_lstadd_back(cutting, new_node);
}
