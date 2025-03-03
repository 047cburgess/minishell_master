#include "minishell.h"

// This function builds a command table, receiving pointer to table, pointer to first token in the cmd, and data
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
	node->av =  get_av(tokens, node->ac);
	if (!node->av)
	{
		free(node);
		return (NULL);
	}
	node->fds[0] = STDIN_FILENO;
	node->fds[1] = STDOUT_FILENO;
	node->next = NULL;
	node->error = 0;
	return (node);
}

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

void	command_del_node(t_command *cmd)
{
	free(cmd->av);
	cmd->av = NULL;
	cmd->tokens = NULL;
	free(cmd);
}

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

t_command	*command_lst_last(t_command *head)
{
	if (!head)
		return (NULL);
	while (head->next != NULL)
		head = head->next;
	return (head);
}

void	log_str_array(char **array)
{
	if (array == NULL)
	{
		ft_dprintf(g_log, "array is null\n");
		return ;
	}
	int i = 0;
	while (array[i])
	{
		ft_dprintf(g_log, "\tav[%i]->[%s]\n", i, array[i]);
		i++;
	}
}

void	print_command_list(t_command *head)
{
	t_command	*current;

	current = head;
	int	i = 1;
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
