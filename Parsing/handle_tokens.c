/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:57:47 by caburges          #+#    #+#             */
/*   Updated: 2025/03/10 13:58:11 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	handle_word_token(int *i, char *start, t_token **tokens_list)
{
	t_token	*new_token;

	new_token = get_word_token(start);
	if (!new_token)
		return (FAILURE);
	token_add_back(tokens_list, new_token);
	*i += ft_strlen(new_token->content);
	return (SUCCESS);
}
