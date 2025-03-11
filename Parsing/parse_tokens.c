#include "minishell.h"
#include "ft_dprintf.h"

// NORM OK
int	type_is_redirection(int type)
{
	if (type == RD_APPEND)
		return (1);
	else if (type == RD_IN)
		return (1);
	else if (type == RD_OUT)
		return (1);
	else if (type == RD_HEREDOC)
		return (1);
	return (0);
}

// NORM OK
// IF 1st or last token is a pipe
// If two pipes in a row
int	check_pipe_syntax(t_token *tokens)
{
	t_token	*current;
	t_token	*last;

	current = tokens;
	last = token_lst_last(tokens);
	if (current->type == PIPE || last->type == PIPE)
		return (FAILURE);
	while (current && current->next)
	{
		if (current->type == PIPE && current->next->type == PIPE)
			return (FAILURE);
		current = current->next;
	}
	return (SUCCESS);
}

// NORM OK
// If the token after a redirection is not a word
int	check_valid_redirections(t_token *head)
{
	t_token	*curr;

	curr = head;
	while (curr && curr->next)
	{
		if (type_is_redirection(curr->type) && curr->next->type != WORD)
		{
			ft_dprintf(2, "%s '%s'\n", ER_MSG_SYNTX_TKN, curr->next->content);
			return (FAILURE);
		}
		curr = curr->next;
	}
	return (SUCCESS);
}

// NORM OK
int	check_token_syntax(t_token *tokens)
{
	t_token	*last;

	last = token_lst_last(tokens);
	if (!check_pipe_syntax(tokens))
	{
		if (tokens && tokens->next)
			ft_dprintf(2, "%s '%s'\n", ER_MSG_SYNTX_TKN, tokens->next->content);
		else
			ft_dprintf(2, "%s '%s'\n", ER_MSG_SYNTX_TKN, tokens->content);
		return (FAILURE);
	}
	if (!check_valid_redirections(tokens))
		return (FAILURE);
	if (type_is_redirection(last->type))
	{
		ft_dprintf(2, "%s 'newline'\n", ER_MSG_SYNTX_TKN);
		return (FAILURE);
	}
	return (SUCCESS);
}
