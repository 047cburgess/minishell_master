
#include "minishell.h"

// get key = delimited by: $, spaces, quotes, end of string
// get value = ft_getenv

// gestion des cas où l'expansion est contenue dans simples quotes (écrire litteralement)
static void handle_simple_quotes(t_list **cutting, char *line, int *i)
{
	int 	start;
	char 	*new_line;
	t_list	*new_node;

	start = *i;
	(*i)++;
	while (line[*i] && line[*i] != '\'')
		(*i)++;
	if (line[*i] == '\'')
		(*i)++;
	new_line = ft_substr(line, start, *i - start);
	new_node = ft_lstnew(new_line);
	ft_lstadd_back(cutting, new_node);
}

//Fonction pour trouver la key (nom expansion) dans env
static char	*find_key(char *line, int i)
{
	char	*key;
	int 	start;

	start = i;
	//while (line[i] && line[i] != '$' && line[i] != ' ' && line[i] != '\"')
	while (line[i] && ft_isalnum(line[i]) && line[i] != '_')
		i++;
	key = ft_substr(line, start, i - start);
	return (key);
}

// convertion les expansions avec le contenu de la variable
static void convert_expansion(t_env *env, t_list **cutting, char *line, int *i)
{
	char *key;
	char *var_content;
	char *expansion;
	t_list	*new_node;

	key = find_key(line, *i + 1);
	printf("KEY = %s\n", key);
	var_content = ft_getenv(env, key);
	//printf("valeur extension = %s\n", var_content);
	if (var_content != NULL)
	{
		expansion = ft_strdup(var_content);
		new_node = ft_lstnew(expansion);
	}
	else
	{
		printf("Expansion [%s] doens't exist.\n", key);
		expansion = ft_strdup("");
		new_node = ft_lstnew(expansion);
	}
	ft_lstadd_back(cutting, new_node);
	*i += ft_strlen(key) + 1;
	free(key);
}

// gestion des cas où l'expansion est située dans des doubles quotes
static void handle_double_quotes(t_env *env, t_list **cutting, char *line, int *i)
{
	int 	start;
	char 	*new_line;
	t_list	*new_node;

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
		(*i)++;
	new_line = ft_substr(line, start, *i - start);
	new_node = ft_lstnew(new_line);
	ft_lstadd_back(cutting, new_node);
}

// gestion noeuds contenants juste du texte simple sans quotes ni expansion
static void handle_simple_text(t_list **cutting, char *line, int *i)
{
	int start;
	char 	*new_line;
	t_list	*new_node;

	start = *i;
	while (line[*i] && line[*i] != '$' && line[*i] != '\"' && line[*i] != '\'')
		(*i)++;
	new_line = ft_substr(line, start, *i - start),
	new_node = ft_lstnew(new_line);
	ft_lstadd_back(cutting, new_node);
}

// fonction pour reconstruire la nouvelle chaine à partir de la liste chainée cutting
static char	*join_list(t_list *lst)
{
	char	*new_line;
	char	*temp;
	int		len;
	t_list	*current;

	len = 0;
	current = lst;
	while (current)
	{
		len += ft_strlen((char *)current->content);
		current = current->next;
	}
	new_line = malloc(len + 1);
	if (!new_line)
		return (NULL);
	new_line[0] = '\0';
	current = lst;
	while (current)
	{
		temp = new_line;
		new_line = ft_strjoin(new_line, (char *)current->content);
		free(temp);
		current = current->next;
	}
	return (new_line);
}

// fonction globale - reconstitution d'une nouvelle line en fonctions des differents cas possibles
char	*expansion_line(t_env *env, char *line)
{
	t_list 	*cutting = NULL;
	char    *new_line = NULL;
	int 	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'')
		 	handle_simple_quotes(&cutting, line, &i);
		else if (line[i] == '\"')
			handle_double_quotes(env, &cutting, line, &i);
		else if (line[i] == '$' && line[i + 1])
			convert_expansion(env, &cutting, line, &i);
		else
			handle_simple_text(&cutting, line, &i);
	}
	new_line = join_list(cutting);
	if (!new_line)
	{
		ft_putstr_fd("Allocation new_line ac extension failed\n", 2);
		ft_lstclear(&cutting, free);
		return (NULL);
	}
	//printf("LIGNE AVEC EXPENSION = %s\n", new_line);
	ft_lstclear(&cutting, free);
	return (new_line);
}

