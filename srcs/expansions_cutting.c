/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_cutting.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 12:48:02 by alize             #+#    #+#             */
/*   Updated: 2025/02/24 15:24:47 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Remplace par expension
char *convert_expansion(t_env *env, char *line, int *i)
{
    char	*expansion;
    char	*var_content;
    char	*key;

	if (line[*i] == '$')
	{
		if (line[*i + 1] == '$')
		{
			expansion = ft_strdup("$$");
			*i += 2;
			return (expansion);
		}
	}
 	key = find_key(line, *i + 1);
	if (!key)
		return (ft_strdup(""));
    var_content = ft_getenv(env, key);
	//printf("var content = %s\n", var_content);
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

static void	append_substring(char **result, char *line, int start, int end)
{
	char	*temp;
	char	*new_line;

	if (end > start)
    {
        temp = ft_substr(line, start, end - start);
        new_line = ft_strjoin(*result, temp);
        free(result);
		free(temp);
        *result = new_line;
	}
}
// Gestion pour quotes doubles
void	handle_double_quotes(t_env *env, t_list **cutting, char *line, int *i)
{
	char *expansion;
    char *result;
	int start;

    (*i)++;
    result = ft_strdup("");
    start = *i;
    while (line[*i] && line[*i] != '\"')
    {
        if (line[*i] == '$' && line[*i + 1] && line[*i + 1] != '\"')
        {
			append_substring(&result, line, start, *i);
            expansion = convert_expansion(env, line, i);
			append_substring(&result, expansion, 0, ft_strlen(expansion));
			free(expansion);
            start = *i;
        }
        else
            (*i)++;
    }
    append_substring(&result, line, start, *i);
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
