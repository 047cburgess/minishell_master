#include "minishell.h"
#include "ft_dprintf.h"

int	type_is_redirection(int type)
{
	return (type == RD_APPEND || type == RD_IN || type == RD_OUT || type == RD_HEREDOC);
}

int	check_pipe_syntax(t_token *tokens)
{
	t_token	*current = tokens;
	t_token	*last = token_lst_last(tokens);

	// IF 1st or last token is a pipe
	if (current->type == PIPE || last->type == PIPE)
		return (FAILURE);
	
	// If two pipes in a row
	while (current && current->next)
	{
		if (current->type == PIPE && current->next->type == PIPE)
			return (FAILURE);
		current = current->next;
	}
	return (SUCCESS);
}

int	check_valid_redirections(t_token *head)
{
	t_token *current = head;
	t_token *last = token_lst_last(head);

	// If the token after a redirection is not a word
	while (current && current->next)
	{
		if (type_is_redirection(current->type) && current->next->type != WORD)
		{
			ft_dprintf(2, "minishell: syntax error near unexpected token '%s'\n", current->content);
			return (FAILURE);
		}
		current = current->next;
	}
	// If the last token in list is a redirection
	if (type_is_redirection(last->type))
	{
		ft_dprintf(2, "minishell: syntax error near unexpected token 'newline'\n");
		return (FAILURE);
	}
	return (SUCCESS);
}

int	check_token_syntax(t_token *tokens)
{
	if (!check_pipe_syntax(tokens))
	{
		ft_putendl_fd("invalid '|' syntax", 2);
		return (FAILURE);
	}
	if (!check_valid_redirections(tokens))
		return (FAILURE);
	return (SUCCESS);
}
