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

// This function goes through the tokens and processes redirections from left to right
int	handle_redirections(t_data *data, t_command *cmd, int *in_out)
{
	int log_file = data->log;
	t_token *token;

	token = cmd->tokens;
	while (token->next != NULL && token->type != PIPE)
	{
		dprintf(log_file, "[%s] entering handle redirection\n", token->content);
		if (type_is_redirection(token->type))
		{
			dprintf(log_file, "[%s] identified as redirection\n", token->content);
			if (is_redirection_in(token->type))
			{
				handle_redirection_in(data, cmd, in_out, token);
				if (cmd->error != 0)
				{
					ft_dprintf(data->log, "rd in failed: errno: %i\n", cmd->error);
					return (1);
				}
			}
			else
			{
				handle_redirection_out(data, cmd, in_out, token);
				if (cmd->error != 0)
					return (1);
			}
		}
		token = token->next;
	}
	return (0);
}

// Manages input redirections
int	handle_redirection_in(t_data *data, t_command *cmd, int *in_out, t_token *token)
{
	int log_file = data->log;
	if (token->type == RD_IN)
	{
		dprintf(log_file, "[%s] identified as RD_IN\n", token->content);
		in_out[0] = open(token->next->content, O_RDONLY);
		if (in_out[0] == -1)
		{
			ft_dprintf(2, "minishell: %s: %s\n", token->next->content, strerror(errno));
			cmd->error = errno;
			return (errno);
		}
		dprintf(log_file, "Opened '%s'\n", token->next->content);
		if (dup2(in_out[0], STDIN_FILENO) == -1)
		{
			ft_dprintf(2, "minishell: %s\n", strerror(errno));
			cmd->error = errno;
			return (errno);
		}
		if (in_out[0] != STDIN_FILENO)
			close(in_out[0]);
		dprintf(log_file, "Closed '%s'\n", token->next->content);
		return (0);
	}
	else
		ft_dprintf(2, "Not managing heredocs yet, need to see how and where to manage\n");
	return (0);
}

// Manages output redirections
int	handle_redirection_out(t_data *data, t_command *cmd, int *in_out, t_token *token)
{
	int log_file = data->log;
	if (token->type == RD_OUT)
	{
		dprintf(log_file, "[%s] identified as RD_OUT\n", token->content);
		in_out[1] = open(token->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else
	{
		dprintf(log_file, "[%s] identified as RD_APPEND\n", token->content);
		in_out[1] = open(token->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);

	}
	if (in_out[1] == -1)
	{
		ft_dprintf(2, "minishell: %s: %s\n", token->next->content, strerror(errno));
		cmd->error = errno;
		return (errno);
	}
	dprintf(log_file, "Redirecting output to '%s'\n", token->next->content);
	if (dup2(in_out[1], STDOUT_FILENO) == -1)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		cmd->error = errno;
		return (errno);
	}
	dprintf(log_file, "Closing '%s'\n", token->next->content);
	if (in_out[1] != STDOUT_FILENO)
	{
		close(in_out[1]);
		dprintf(log_file, "Closed '%s'\n", token->next->content);
	}
	return (0);
}
