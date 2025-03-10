#include "minishell.h"

int	handle_operator_token(int *i, char *start, t_token **tokens_list)
{
	t_token	*new_token;
	
	new_token = get_operator_token(start);
	if (!new_token)
		return (FAILURE);
	token_add_back(tokens_list, new_token);
	*i += ft_strlen(new_token->content);
	return (SUCCESS);
}

int	handle_quoted_token(int *i, char *start, t_token **tokens_list)
{
	t_token	*new_token;
	
	new_token = get_quoted_token(start);
	if (!new_token)
		return (FAILURE);
	token_add_back(tokens_list, new_token);
	*i += ft_strlen(new_token->content);
	return (SUCCESS);
}

int	handle_unquoted_token(int *i, char *start, t_token **tokens_list)
{
	t_token	*new_token;
	
	new_token = get_unquoted_token(start);
	if (!new_token)
		return (FAILURE);
	token_add_back(tokens_list, new_token);
	*i += ft_strlen(new_token->content);
	return (SUCCESS);

}
