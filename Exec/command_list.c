#include "minishell.h"

// This function builds a command table, receiving pointer to table, 
// pointer to first token in the cmd, and data
// NORM OK
t_command	*new_command_table(t_token *tokens)
{
	t_command	*node;

	node = ft_calloc(1, sizeof(t_command));
	if (!node)
	{
		return (NULL);
	}
	node->tokens = tokens;
	node->ac = get_ac(tokens);
	node->av = get_av(tokens, node->ac);
	if (!node->av)
	{
		free(node);
		return (NULL);
	}
	node->fds[0] = -1;
	node->fds[1] = -1;
	node->pid = -1;
	node->next = NULL;
	node->error = 0;
	return (node);
}

// NORM OK
void	command_add_back(t_command **head, t_command *new)
{
	t_command	*current;

	if (!head || !new)
		return ;
	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	current = *head;
	while (current->next)
	{
		current = current->next;
	}
	current->next = new;
}

// NORM OK
void	command_del_node(t_command *cmd)
{
	free(cmd->av);
	cmd->av = NULL;
	cmd->tokens = NULL;
	free(cmd);
}

// NORM OK
void	command_lst_clear(t_command **head)
{
	t_command	*temp;

	while (*head)
	{
		temp = (*head)->next;
		command_del_node(*head);
		*head = temp;
	}
	*head = NULL;
}

// NORM OK
t_command	*command_lst_last(t_command *head)
{
	if (!head)
		return (NULL);
	while (head->next != NULL)
		head = head->next;
	return (head);
}

// DEBUG ONLY
void	log_str_array(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
	{
		ft_dprintf(g_log, "array is null\n");
		return ;
	}
	while (array[i])
	{
		ft_dprintf(g_log, "\tav[%i]->[%s]\n", i, array[i]);
		i++;
	}
}

// DEBUG ONLY
void	print_command_list(t_command *head)
{
	t_command	*current;
	int			i;

	i = 1;
	current = head;
	ft_dprintf(g_log, "----- COMMAND LIST -----\n");
	while (current)
	{
		ft_dprintf(g_log, "COMMAND %i\n", i);
		ft_dprintf(g_log, "\tARGV: \n");
		log_str_array(current->av);
		ft_dprintf(g_log, "\tAC: %i\n", current->ac);
		ft_dprintf(g_log, "\tin_fd: %i\n", current->fds[0]);
		ft_dprintf(g_log, "\tout_fd: %i\n", current->fds[1]);
		if (current->next)
			ft_dprintf(g_log, "\tNEXT -> %s\n", current->next->av[0]);
		current = current->next;
		i++;
	}
	ft_dprintf(g_log, "\n");
}
