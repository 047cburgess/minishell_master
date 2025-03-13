/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_cutting.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:04:47 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/11 16:14:52 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_dollar_alone(t_data *data, int *i)
{
	t_list	*new_node;
	char 	*dollar;

	dollar = ft_strdup("$");
	if (!dollar)
		return (1);
	new_node = ft_lstnew(dollar);
	if (!new_node)
	{
		free(dollar);
		return (1);
	}
	ft_lstadd_back(&data->cutting, new_node);
	(*i)++;
	return (0);
}

int	handle_exit_extansion(t_data *data, char *line, int *i)
{
	char	*expansion;
	char	*result;
	t_list	*new_node;

	if (line[*i] == '$' && line[*i + 1] == '?')
	{
		(*i) += 2;
		result = ft_itoa(data->status);
		if (!result)
			return (1);
		expansion = ft_strdup(result);
		free(result);
		new_node = ft_lstnew(expansion);
		if (new_node)
			ft_lstadd_back(&data->cutting, new_node);
		else
			return (1);
		return (0);
	}
	return (0);
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

int	handle_simple_quotes(t_data *data, char *line, int *i)
{
	int		start;
	char	*new_line;
	t_list	*new_node;

	(*i)++;
	start = *i;
	while (line[*i] && line[*i] != '\'')
		(*i)++;
	if (line[*i] == '\'')
	{
		new_line = ft_substr(line, start, *i - start);
		if (!new_line)
			return (1);
		new_node = ft_lstnew(new_line);
		if (!new_node)
		{
			free(new_line);
			return (1);
		}
		ft_lstadd_back(&data->cutting, new_node);
		(*i)++;
	}
	return (0);
}

int	handle_simple_text(t_data *data, char *line, int *i)
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
		return (1);
	new_node = ft_lstnew(new_line);
	if (!new_node)
	{
		free(new_line);
		return (1);
	}
	ft_lstadd_back(&data->cutting, new_node);
	return (0);
}
