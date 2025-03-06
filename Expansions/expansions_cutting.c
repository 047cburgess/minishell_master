/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_cutting.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:04:47 by alsuchon          #+#    #+#             */
/*   Updated: 2025/03/06 13:13:00 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_status(t_data *data, char *line, int *i)
{
	char	*expansion;
	char	*result;

	if (line[*i + 1] == '?' && line[*i + 2] == '\0')
	{
		(*i) += 2;
		result = ft_itoa(data->status);
		if (!result)
			return (NULL);
		expansion = ft_strdup(result);
		free(result);
		return (expansion);
	}
	return (NULL);
}

char	*convert_expansion(t_data *data, char *line, int *i)
{
	char	*expansion;
	char	*var_content;
	char	*key;
	int		start;

	expansion = expand_status(data, line, i);
	if (expansion)
		return (expansion);
	start = *i;
    key = find_key(line, *i + 1);
	if (!key)
		return (NULL);
	if (key_is_valid(key) == false)
	{
		*i += ft_strlen(key) + 1;
		free(key);
		return (ft_substr(line, start, *i - start));
	}
	var_content = ft_getenv(data->env, key);
	free(key);
	if (!var_content)
		return (ft_strdup(""));
	expansion = ft_strdup(var_content);
	*i += ft_strlen(key) + 1;
	return (expansion);
}

void	handle_simple_quotes(t_list **cutting, char *line, int *i)
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
			return ;
		new_node = ft_lstnew(new_line);
		if (!new_node)
		{
			free(new_line);
			return ;
		}
		ft_lstadd_back(cutting, new_node);
		(*i)++;
	}
}

char static	*expansion_doubles_quotes(t_data *data, char *line, int *i, char *result)
{
	char	*expansion;
	char	*temp;

	expansion = convert_expansion(data, line, i);
	if (!expansion)
		return (result);
	temp = ft_strjoin(result, expansion);
	free(result);
	free(expansion);
	if (!temp)
		return (NULL);
	return (temp);
}

char static	*add_substring(char *result, char *line, int start, int end)
{
	char	*temp;
	char	*new_line;

	temp = ft_substr(line, start, end - start);
	if (!temp)
		return (free(result), NULL);
	new_line = ft_strjoin(result, temp);
	free(temp);
	if (!new_line)
		return (free(result), NULL);
	free(result);
	return (new_line);
}

void	handle_double_quotes(t_data *data, t_list **cutting, char *line, int *i)
{
	char	*result;
	int		start;

	(*i)++;
	result = ft_strdup("");
	if (!result)
		return ;
	start = *i;
	while (line[*i] && line[*i] != '\"')
	{
    	if (line[*i] == '$' && line[*i + 1] && line[*i + 1] != '\"')
		{
			if (*i > start)
			{
				result = add_substring(result, line, start, *i);
				if (!result)
					return ;
			}
			result = expansion_doubles_quotes(data, line, i, result);
			if (!result)
				return ;
			start = *i;
		}
		else
			(*i)++;
	}
	if (*i > start)
	{
		result = add_substring(result, line, start, *i);
		if (!result)
			return ;
	}
	ft_lstadd_back(cutting, ft_lstnew(result));
	if (line[*i] == '\"')
		(*i)++;
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
