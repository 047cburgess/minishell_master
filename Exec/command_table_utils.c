#include "minishell.h"

// This function returns the command name / program name (av[0])
char	*get_command(t_token *list)
{
	t_token	*current;
	t_token *prev = NULL;

	current = list;
	if (current->type == WORD)
		return (current->content);
	prev = current;
	current = current->next;
	while (current != NULL && current->type != PIPE)
	{	
		if (current->type == WORD && prev->type == WORD)
			return (current->content);
		prev = current;
		current = current->next;
	}
	return (NULL);
}

int	is_builtin(char **av)
{
	if (av == NULL || *av == NULL)
		return (FALSE);
	if (ft_strcmp(av[0], "cd") == 0)
		return (TRUE);
	if (ft_strcmp(av[0], "echo") == 0)
		return (TRUE);
	if (ft_strcmp(av[0], "pwd") == 0)
		return (TRUE);
	if (ft_strcmp(av[0], "export") == 0)
		return (TRUE);
	if (ft_strcmp(av[0], "unset") == 0)
		return (TRUE);
	if (ft_strcmp(av[0], "env") == 0)
		return (TRUE);
	else
		return (FALSE);
}

// This function returns the number of arguments for a command (its ac)
int	get_ac(t_token *command_list)
{
	t_token *current;
	t_token *prev = NULL;
	int	ac = 0;

	current = command_list;
	if (current->type == WORD)
	{
		ac++;
	}
	prev = current;
	current = current->next;
	while (current != NULL && current->type != PIPE)
	{
		if (current->type == WORD && prev->type == WORD)
		{
			ac++;
		}
		prev = current;
		current = current->next;
	}
	return (ac);
}

// This function returns the command's argv, NULL if malloc error , empty array if no commands/args
char	**get_av(t_token *tokens, int ac)
{
	char	**av;
	t_token *current;
	t_token *prev;
	int	i;

	av = ft_calloc((ac + 1), sizeof(char *));
	if (!av)
		return (NULL);
	current = tokens;
	i = 0;
	if (current->type == WORD)
	{
		av[i] = current->content;
		if (!av[i])
			return (free(av), NULL);
		i++;
	}
	prev = current;
	current = current->next;
	while (current != NULL && current->type != PIPE)
	{
		if (current->type == WORD && prev->type == WORD)
		{
			av[i] = current->content;
			if (!av[i])
				return (free(av), NULL);
			i++;
		}
		prev = current;
		current = current->next;
	}
	return (av);
}

// This function converts minishell env list back to an array
char	**env_to_array(t_env *env_head)
{
	int	list_size;
	int	strings_len;
	char	**array;
	int	i;

	list_size = env_lst_size(env_head);
	array = ft_calloc(list_size + 1, sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	while (i < list_size)
	{
		strings_len = ft_strlen(env_head->key) + ft_strlen(env_head->value);
		array[i] = ft_calloc(strings_len + 2, sizeof(char));
		if (!array[i])
		{
			return (free_str_array(array, i), NULL);
		}
		ft_strcpy(array[i], env_head->key);
		ft_strlcat(array[i], "=", strings_len + 2);
		ft_strlcat(array[i], env_head->value, strings_len + 2);
		i++;
		env_head = env_head->next;
	}
	return (array);
}
