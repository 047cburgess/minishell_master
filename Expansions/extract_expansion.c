/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:03:06 by alsuchon          #+#    #+#             */
/*   Updated: 2025/02/27 17:10:54 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <ctype.h>

char *find_key(char *line, int i)
{
    int		start;
    char	*key;

	start = i;
    while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
        i++;
    key = ft_substr(line, start, i - start);
    printf("KEY = %s\n", key);
    return (key);
}

static char *join_list(t_list *lst)
{
    char	*new_line;
	char 	*temp;
    t_list	*current;
	// t_list	*print;

	//  print = lst;
	//  printf("CUTTINGS: ");
	//  while (print)
	//  {	
	//  	printf("[%s]->", (char*)print->content);
	//  	print = print->next;
	//  }
	//  printf("\n");
	current = lst;
	new_line = ft_strdup("");
    while (current)
    {
    	printf("current: %s\n", (char *)current->content);
        temp = new_line;
    	printf("joining: %s\n", temp);
        new_line = ft_strjoin(new_line, (char *)current->content);
    	printf("joining: %s\n", new_line);
        free(temp);
        current = current->next;
    }
    printf("finished join");
    return (new_line);
}

// Fonction principale qui gère l'expansion dans la ligne
char *expansion_line(t_env *env, char *line)
{
    t_list *cutting = NULL;
	t_list	*new_node;
	char 	*new_line;
	char 	*expansion;
    int 	i;

	i = 0;
    while (line[i])
    {
        if (line[i] == '\'')
            handle_simple_quotes(&cutting, line, &i);
        else if (line[i] == '\"')
            handle_double_quotes(env, &cutting, line, &i);
	else if (line[i] == '$' && line[i + 1])
	{
            expansion = convert_expansion(env, line, &i);
			new_node = ft_lstnew(expansion);
    		ft_lstadd_back(&cutting, new_node);
		}
	else if (line[i] == '$' && line[i + 1 == '\0'])
	{
		new_node = ft_lstnew(ft_strdup("$"));
		ft_lstadd_back(&cutting, new_node);
		i++;
	}
    else 
        handle_simple_text(&cutting, line, &i);
    }
    new_line = join_list(cutting);
    ft_lstclear(&cutting, free);
    return (new_line);
}

// Applique l'expansion sur un token 
char *expand_token(t_env *env, char *content)
{
    if (!content)
        return (NULL);
	else
		return (expansion_line(env, content));
    // if (expansion_needed(content))
    //     return (expansion_line(env, content));
    //return (ft_strdup(content));
}

// Fonction principale pour gérer les expansions dans la liste t_token
int handle_expansions(t_data *data, t_env *env)
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
