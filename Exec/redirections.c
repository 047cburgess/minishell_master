/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 18:06:00 by caburges          #+#    #+#             */
/*   Updated: 2025/02/28 18:06:01 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection_out(int token_type)
{
	return (token_type == RD_APPEND || token_type == RD_OUT);
}

// This function goes through the tokens and processes 
// redirections from left to right
int	handle_redirections(t_command *cmd, int *in_out)
{
	t_token	*token;

	if (cmd->error != 0)
		return (1);
	token = cmd->tokens;
	while (token->next != NULL && token->type != PIPE)
	{
		if (is_redirection_in(token->type))
		{
			if (!handle_redirection_in(in_out, token))
				cmd->error = 1;
		}	
		else if (is_redirection_out(token->type))
		{
			if (!handle_redirection_out(in_out, token))
				cmd->error = 1;
		}
		if (cmd->error)
			return (1);
		token = token->next;
	}
	return (0);
}

// Manages input redirections
int	handle_redirection_in(int *in_out, t_token *token)
{
	if (token->type == RD_IN || token->type == RD_HEREDOC)
	{
		in_out[0] = open(token->next->content, O_RDONLY);
		if (in_out[0] == -1)
		{
			ft_dprintf(2, ER_OPEN, token->next->content, strerror(errno));
			return (0);
		}
		if (dup2(in_out[0], STDIN_FILENO) == -1)
		{
			ft_dprintf(2, "minishell: %s\n", strerror(errno));
			return (0);
		}
		if (in_out[0] != STDIN_FILENO)
			ft_close(&in_out[0]);
		return (1);
	}
	return (1);
}

int	get_flag(int token_type)
{
	if (token_type == RD_OUT)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	else
		return (O_WRONLY | O_CREAT | O_APPEND);
}

// Manages output redirections
int	handle_redirection_out(int *in_out, t_token *token)
{
	if (token->type == RD_OUT)
		in_out[1] = open(token->next->content, get_flag(token->type), 0644);
	else
		in_out[1] = open(token->next->content, get_flag(token->type), 0644);
	if (in_out[1] == -1)
	{
		ft_dprintf(2, ER_OPEN, token->next->content, strerror(errno));
		return (0);
	}
	if (dup2(in_out[1], STDOUT_FILENO) == -1)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		return (0);
	}
	if (in_out[1] != STDOUT_FILENO)
		close(in_out[1]);
	return (1);
}
