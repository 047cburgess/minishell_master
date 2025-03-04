
#include "minishell.h"

// This function prints the env list like bash
void	print_env_list(t_env *env_head)
{
	while (env_head != NULL)
	{
		printf("%s=%s\n", env_head->key, env_head->value);
		env_head = env_head->next;
	}
}

// This function creates and returns a new node according to given key and value
t_env	*env_node_new(char *key, char *value)
{
	t_env	*new_node;

	if (!key)
		return (NULL);
	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = key;
	new_node->value = value;
	new_node->no_value = 0;
	new_node->next = NULL;
	return (new_node);
}

// This function deletes a node from the environment list (does not connect the list back together)
void	env_delone(t_env *node)
{
	free(node->key);
	free(node->value);
	node->key = NULL;
	node->value = NULL;
	node->no_value = 0;
	free(node);
}

// This function deletes / frees the whole env list
void	env_list_clear(t_env **env_list)
{
	t_env	*temp;

	while (*env_list)
	{
		temp = (*env_list)->next;
		env_delone(*env_list);
		*env_list = temp;
	}
	*env_list = NULL;
}

// This function returns the last element of the environment list
t_env	*env_last(t_env *env_head)
{
	if (!env_head)
		return (NULL);
	while (env_head->next != NULL)
		env_head = env_head->next;
	return (env_head);
}

// This function adds a new node (already created) to the back of the env list
void	env_add_back(t_env **env_head, t_env *new_node)
{
	t_env	*last;

	if (*env_head == NULL)
		*env_head = new_node;
	else
	{
		last = env_last(*env_head);
		last->next = new_node;
	}
}

// This function returns the list size of minishell env
int	env_lst_size(t_env *env)
{
	int	size;

	size = 0;
	while (env != NULL)
	{
		size++;
		env = env->next;
	}
	return (size);
}