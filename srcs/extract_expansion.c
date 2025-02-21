
#include "minishell.h"

int	expansion_needed(char *content)
{
	char	*ptr = content;

	if  (ft_strchr(content, '$') == NULL)
		return (FALSE);
	if (ft_strcmp(content, "$") == 0)
		return (FALSE);
	while(*ptr)
	{
		if (*ptr == '\'')
			ptr = ft_strchr(ptr + 1, '\'') + 1;
		else if (*ptr == '\"')
		{
			ptr++;
			while (*ptr && *ptr != '\"')
			{
				if (*ptr == '$' && *(ptr + 1) != '\"')
					return (TRUE);
				ptr++;
			}
		}
		else if (*ptr == '$' && *(ptr + 1))
			return (TRUE);
		ptr++;
	}
	return (FALSE);
}

static char *find_key(char *line, int i)
{
    int		start;
    char	*key;

	start = i;
    while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
        i++;
    key = ft_substr(line, start, i - start);
    return (key);
}

// Remplace par expension
static void convert_expansion(t_env *env, t_list **cutting, char *line, int *i)
{
    t_list *new_node;
    char *expansion;
    char *var_content;
    char *key;

    key = find_key(line, *i + 1);
    var_content = ft_getenv(env, key);
    expansion = var_content;
	if (var_content != NULL)
		ft_strdup(var_content);
	else
		ft_strdup("");
    new_node = ft_lstnew(expansion);
    ft_lstadd_back(cutting, new_node);
    *i += ft_strlen(key) + 1;
    free(key);
}

// // Gestion des quotes simples

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
    }
    if (line[*i] == '\'')
    {
        (*i)++;
    }
}

// Gestion des quotes doubles
static void handle_double_quotes(t_env *env, t_list **cutting, char *line, int *i)
{
	char 	*new_line;
	t_list	*new_node;
    int 	start;

	start = *i;
    (*i)++;
    while (line[*i] && line[*i] != '\"')
    {
        if (line[*i] == '$' && line[*i + 1] && line[*i + 1] != '\"')
        {
			new_line = ft_substr(line, start, *i - start);
			new_node = ft_lstnew(new_line);
			ft_lstadd_back(cutting, new_node);
            convert_expansion(env, cutting, line, i);
            start = *i;
        }
        else
            (*i)++;
    }
    if (line[*i] == '\"')
	{
        (*i)++;
	}
	new_line = ft_substr(line, start, *i - start);
	new_node = ft_lstnew(new_line);
    ft_lstadd_back(cutting, new_node);
}

// Texte simple
static void handle_simple_text(t_list **cutting, char *line, int *i)
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

static char *join_list(t_list *lst)
{
    char	*new_line;
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
        char *temp = new_line;
        new_line = ft_strjoin(new_line, (char *)current->content);
        free(temp);
        current = current->next;
    }
    return (new_line);
}

// Fonction principale qui gère l'expansion dans la ligne
char *expansion_line(t_env *env, char *line)
{
    t_list *cutting = NULL;
	char *new_line;
    int i;

	i = 0;
    while (line[i])
    {
        if (line[i] == '\'')
            handle_simple_quotes(&cutting, line, &i);
        else if (line[i] == '\"')
            handle_double_quotes(env, &cutting, line, &i);
        else if (line[i] == '$' && line[i + 1] && line[i + 1] != '\'')
            convert_expansion(env, &cutting, line, &i);
        else
            handle_simple_text(&cutting, line, &i);
    }
    new_line = join_list(cutting);
    ft_lstclear(&cutting, free);
    return (new_line);
}

// Applique l'expansion sur un token si nécessaire
char *expand_token(t_env *env, char *content)
{
    if (!content)
        return (NULL);
    if (expansion_needed(content))
        return (expansion_line(env, content));
    return (ft_strdup(content));
}

// Fonction principale pour gérer les expansions dans la liste t_token
int handle_expansions(t_data *data, t_env *env)
{
	t_token *current;
    char    *expanded_content;

   	current = data->tokens_list;
	if (!current)
        return (FAILURE);
	printf("--EXPANSIONS--\n");
    while (current)
    {
        expanded_content = expand_token(env, current->content);
        free(current->content);
        current->content = expanded_content;
		printf("[%s]\n", current->content);
        current = current->next;
    }
    return (SUCCESS);
}