/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_cutting.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 12:48:02 by alize             #+#    #+#             */
/*   Updated: 2025/02/26 18:06:09 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Remplace par expension
char	*convert_expansion(t_env *env, char *line, int *i)
{
	char	*expansion;
	char	*var_content;
	char	*key;

	key = find_key(line, *i + 1);
	if (!key)
		return (ft_strdup(""));
	var_content = ft_getenv(env, key);
	if (var_content != NULL)
		expansion = ft_strdup(var_content);
	else
		expansion = ft_strdup("");
	*i += ft_strlen(key) + 1;
	free(key);
	return (expansion);
}

// Gestion pour quotes simples
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
		new_node = ft_lstnew(new_line);
		ft_lstadd_back(cutting, new_node);
		(*i)++;
	}
}

void	handle_double_quotes(t_env *env, t_list **cutting, char *line, int *i)
{
	char	*expansion;
	char	*result;
	int		start;
	int		j;

	(*i)++;
    result = ft_strdup("");
    start = *i;
    while (line[*i] && line[*i] != '\"')
    {
        if (line[*i] == '$')
        {
            j = *i;
            while (line[j] == '$')
                j++;
            if (line[j] && (ft_isalnum(line[j]) || line[j] == '_'))
            {
                append_substring(&result, line, start, j - 1);
                *i = j - 1; 
                expansion = convert_expansion(env, line, i);
                append_substring(&result, expansion, 0, ft_strlen(expansion));
                free(expansion);
                start = *i;
            }
            else
            {
                append_substring(&result, line, start, j);
                *i = j;
                start = *i;
            }
        }
        else
        {
            (*i)++;
        }
    }
    append_substring(&result, line, start, *i);
    ft_lstadd_back(cutting, ft_lstnew(result));
    if (line[*i] == '\"')
        (*i)++;
}

//gestion texte sans quotes
void handle_simple_text(t_env *env, t_list **cutting, char *line, int *i)
{
    char    *result;
    char    *expansion;
    int     start;
    int     j;
    int     dollar;
	char	*expansion_trim;

    result = ft_strdup("");
    start = *i;
    while (line[*i] && line[*i] != '\"' && line[*i] != '\'')
    {
        if (line[*i] == '$')
        {
            j = *i;
            dollar = 0;
            while (line[j] == '$')
            {
                dollar++;
                j++;
            }
            if (dollar > 0)
            {
                append_substring(&result, line, start, *i + (dollar - 1));
                *i += (dollar - 1);
                start = *i;
            }
            if (line[j] && (ft_isalnum(line[j]) || line[j] == '_'))
            {
                append_substring(&result, line, start, *i);
                *i = j - 1;
                expansion = convert_expansion(env, line, i);
				expansion_trim = ft_strtrim(expansion, " ");
                append_substring(&result, expansion_trim, 0, ft_strlen(expansion_trim));
                free(expansion);
				free(expansion_trim);
                start = *i + 1;
            }
            else
            {
                append_substring(&result, line, start, j);
				*i= j;
				start = *i;
			}
		}
		else
		{
			(*i)++;
		}
	}
	append_substring(&result, line, start, *i);
	ft_lstadd_back(cutting, ft_lstnew(result));
}
