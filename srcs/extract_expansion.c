/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 12:48:11 by alize             #+#    #+#             */
/*   Updated: 2025/02/26 16:30:06 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*join_list(t_list *lst)
{
    char	*new_line;
	char 	*temp;
    t_list	*current;

	current = lst;
	new_line = ft_strdup("");
	if (!new_line)
		return (NULL);
    while (current)
    {
        temp = new_line;
        new_line = ft_strjoin(new_line, (char *)current->content);
        free(temp);
		if (!new_line)
			return (NULL);
        current = current->next;
    }
    return (new_line);
}

// Fonction principale qui gère l'expansion dans la ligne
char	*expansion_line(t_env *env, char *line)
{
    t_list *cutting = NULL;
	char 	*new_line;
    int 	i;

	i = 0;
    while (line[i])
    {
        if (line[i] == '\'')
            handle_simple_quotes(&cutting, line, &i);
        else if (line[i] == '\"')
            handle_double_quotes(env, &cutting, line, &i);
		else if (line[i] == '$' && (ft_isalnum(line[i]) || line[i] == '_'))
            convert_expansion(env, line, &i);
        else
            handle_simple_text(env, &cutting, line, &i);
    }
    new_line = join_list(cutting);
    ft_lstclear(&cutting, free);
    return (new_line);
}

// Applique l'expansion sur un token 
char	*expand_token(t_env *env, char *content)
{
    if (!content)
        return (NULL);
	else
		return (expansion_line(env, content));
}

// Fonction principale pour gérer les expansions dans la liste t_token
int	handle_expansions(t_data *data, t_env *env)
{
	t_token *current;
    char    *expanded_content;

   	current = data->tokens_list;
	if (!current)
        return (FAILURE);
	printf("--EXPANSIONS--\n\t");
    while (current)
    {
        expanded_content = expand_token(env, current->content);
		if (expanded_content != current->content)
        	free(current->content);
        current->content = expanded_content;
		printf("[%s]->", current->content);
        current = current->next;
    }
    printf("\n");
    return (SUCCESS);
}
