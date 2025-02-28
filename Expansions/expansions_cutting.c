/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_cutting.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:04:47 by alsuchon          #+#    #+#             */
/*   Updated: 2025/02/28 15:30:47 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Remplace par expension
char	*convert_expansion(t_data *data, char *line, int *i)
{
    char	*expansion;
    char	*var_content;
    char	*key;
	int		start;
	
	if (line[*i + 1] == '?')
	{
		(*i)+= 2;
		expansion = ft_strdup(ft_itoa(data->status));
		return (expansion);
	}
    start = *i;
    key = find_key(line, *i + 1);
    printf("key: %s\n", key);
	/*if (key[0] == '\0')
	{
		*i += 1;
		return (key);
	}*/
	if (key_is_valid(key) == false)
	{
    		*i += ft_strlen(key) + 1;
		free(key);
		return (ft_substr(line, start, *i - start));
	}
    var_content = ft_getenv(data->env, key);
	if (var_content != NULL)
		expansion = ft_strdup(var_content);
	else
		expansion = ft_strdup("");
    *i += ft_strlen(key) + 1;
    free(key);
	return (expansion);
}

void	handle_simple_quotes(t_list **cutting, char *line, int *i)
{
    int     start;
    char    *new_line;
    t_list  *new_node;

    (*i)++;
    start = *i;
    while (line[*i] && line[*i] != '\'')
        (*i)++;
    if (line[*i] == '\'')
    {
        new_line = ft_substr(line, start, *i - start);
        new_node = ft_lstnew(new_line);
        ft_lstadd_back(cutting, new_node);
		(*i)++;
    }
}

void	handle_double_quotes(t_data *data, t_list **cutting, char *line, int *i)
{
	char *expansion;
	char *new_line;
    char *temp;
    char *result;
	int start;

    (*i)++;
    result =ft_strdup("");
    start = *i;
    while (line[*i] && line[*i] != '\"')
    {
        if (line[*i] == '$' && line[*i + 1] && line[*i + 1] != '\"')
        {
            if (*i > start)
            {
                temp = ft_substr(line, start, *i - start);
                new_line = ft_strjoin(result, temp);
                free(result);
				free(temp);
                result = new_line;
			}
            expansion = convert_expansion(data, line, i);
			temp = ft_strjoin(result, expansion);
			free(result);
			free(expansion);
			result = temp;
            start = *i;
        }
        else
            (*i)++;
    }
    if (*i > start)
    {
        temp = ft_substr(line, start, *i - start);
        new_line = ft_strjoin(result, temp);
		free(result);
		free(temp);
        result = new_line;
    }
	ft_lstadd_back(cutting, ft_lstnew(result));
    if (line[*i] == '\"')
        (*i)++;
}

// Texte simple
void	handle_simple_text(t_list **cutting, char *line, int *i)
{
	char 	*new_line;
	t_list	*new_node;
    int 	start;

	start = *i;
    while (line[*i] && line[*i] != '$' && line[*i] != '\"' && line[*i] != '\'')
	{
        (*i)++;
	}
	new_line = ft_substr(line, start, *i - start);
	new_node = ft_lstnew(new_line);
	ft_lstadd_back(cutting, new_node);
}
