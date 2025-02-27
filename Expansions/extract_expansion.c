
#include "minishell.h"
#include <ctype.h>


static char *find_key(char *line, int i)
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

// Remplace par expension
char *convert_expansion(t_env *env, char *line, int *i)
{
    char	*expansion;
    char	*var_content;
    char	*key;

	//if (line[*i] == '$')
	//{
	//	if (line[*i + 1] == '$')
	//	{
	//		expansion = ft_strdup("$$");
	//		*i += 2;
	//		return (expansion);
	//	}
	//}
    key = find_key(line, *i + 1);
	if (key[0] == '\0')
	{
		printf("no key");
		*i += 1;
		return (key);

	}
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

static void handle_simple_quotes(t_list **cutting, char *line, int *i)
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

static void handle_double_quotes(t_env *env, t_list **cutting, char *line, int *i)
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
            expansion = convert_expansion(env, line, i);
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
void handle_simple_text(t_env *env, t_list **cutting, char *line, int *i)
{
	(void)env;
	char 	*new_line;
	t_list	*new_node;
    int 	start;

	start = *i;
    while (line[*i] && line[*i] != '$' && line[*i] != '\"' && line[*i] != '\'')
	{
        (*i)++;
	}
    	//if (line[*i] == '$')
	//	(*i)++;
	new_line = ft_substr(line, start, *i - start);
	new_node = ft_lstnew(new_line);
	ft_lstadd_back(cutting, new_node);
}

static char *join_list(t_list *lst)
{
    char	*new_line;
	char 	*temp;
    t_list	*current;
	 t_list	*print;

	 print = lst;
	 printf("CUTTINGS: ");
	 while (print)
	 {	
	 	printf("[%s]->", (char*)print->content);
	 	print = print->next;
	 }
	 printf("\n");
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
            handle_simple_text(env, &cutting, line, &i);

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
