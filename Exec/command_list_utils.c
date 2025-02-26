#include "minishell.h"

// This function builds a command table, receiving pointer to table, pointer to first token in the cmd, and data
t_command	*new_command_table(t_token *tokens, t_data *data)
{
	t_command	*node;

	node = ft_calloc(1, sizeof(t_command));
	if (!node)
	{
		return (NULL);
	}
	node->tokens = tokens;
	node->ac = get_ac(data->tokens_list);
	node->av =  get_av(tokens, node->ac);
	if (!node->av)
	{
		free(node);
		return (NULL);
	}
	node->fds[0] = STDIN_FILENO;
	node->fds[1] = STDOUT_FILENO;
	node->env = env_to_array(data->env);
	if (!node->env)
	{
		free_str_array(node->av, node->ac);
		free(node);
		return (NULL);
	}
	node->path = NULL;//ft_getenv(data->env, "PATH");
	node->next = NULL;
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
	int	i;

	i = 0;
	while (cmd->env[i])
		i++;
	free_str_array(cmd->av, cmd->ac);
	free_str_array(cmd->env, i);
	cmd->av = NULL;
	cmd->env = NULL;
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

void	print_command_list(t_command *head)
{
	t_command	*current;

	current = head;
	int	i = 1;
	printf("----- COMMAND LIST -----\n");
	while (current)
	{
		printf("\tCOMMAND %i\n", i);
		printf("\tARGV: \n");
		print_str_array(current->av);
		printf("\tAC: %i\n", current->ac);
		printf("\tin_fd: %i\n", current->fds[0]);
		printf("\tout_fd: %i\n", current->fds[1]);
		if (current->next)
			printf("\tNEXT -> %s", current->next->av[0]);
		current = current->next;
	}
	printf("\n");
}
