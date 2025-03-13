/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 14:07:27 by caburges          #+#    #+#             */
/*   Updated: 2025/03/13 14:07:29 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_and_write(char **line, char *delimiter, int fd)
{
	while (g_signal == 0)
	{
		*line = readline("> ");
		if (*line == NULL)
		{
			if (g_signal == 0)
				ft_dprintf(2, ER_HEREDOC_MSG, delimiter);
			break;
		}
		else if (ft_strcmp(*line, delimiter) == 0)
			break;
		write(fd, *line, ft_strlen(*line));
		write(fd, "\n", 1);
		ft_free((void *)line);
	}
}

// stop reading when line contains only delimiter OR ctld is received (null)
int	write_into_heredoc(int fd, char *delimiter)
{
	char	*line;
	int	stdin_dup;

	signal(SIGINT, heredoc);
	stdin_dup = dup(STDIN_FILENO);
	if (stdin_dup == -1)
		return (signal(SIGINT, SIG_IGN), 0);
	line = NULL;
	read_and_write(&line, delimiter, fd);
	ft_free((void *)&line);
	dup2(stdin_dup, STDIN_FILENO);
	close(stdin_dup);
	signal(SIGINT, SIG_IGN);
	if (g_signal != 0)
		return (0);
	return (1);
}

int	set_file_name(char **file_name, int id)
{
	char	buffer[4097];
	char	*hd_id;

	ft_bzero(buffer, sizeof(buffer));
	hd_id = ft_itoa(id);
	if (!hd_id)
		return (0);
	ft_strlcat(buffer, ".heredoc", sizeof(buffer));
	ft_strlcat(buffer, hd_id, sizeof(buffer));
	free(hd_id);
	*file_name = ft_strdup(buffer);
	ft_dprintf(g_log, "filanme: %s\n", file_name);
	if (*file_name == NULL)
		return (0);
	return (1);
}

int	process_heredoc(t_token *delimiter, int id)
{
	char	*file_name;
	int	fd;

	ft_dprintf(g_log, "FUNCTION: PROCESS HEREDOC\n");
	file_name = NULL;
	if (set_file_name(&file_name, id) == FAILURE)
		return (0);
	ft_dprintf(g_log, "filename1: %s\n", file_name);
	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (ft_free((void*)&file_name), 0);
	if (!write_into_heredoc(fd, delimiter->content))
	{
		ft_dprintf(g_log, "unlinking '%s'\n", file_name);
		unlink(file_name);
		ft_free((void*)&file_name);
		close(fd);
		return (0);
	}
	close(fd);
	ft_dprintf(g_log, "closed heredoc %i\n", id);
	free(delimiter->content);
	delimiter->content = file_name;
	ft_dprintf(g_log, "filename2: %s\n", delimiter->content);
	return (1);
}

int	handle_heredocs(t_data *data, t_token *tokens)
{
	int	heredoc_count;
	t_token 	*current_delimiter;
	int	i;

	heredoc_count = get_heredoc_count(tokens);
	if (heredoc_count == 0)
		return (1);
	current_delimiter = tokens;
	i = 0;
	while (i < heredoc_count)
	{
		current_delimiter = get_next_heredoc_delimiter(current_delimiter);
		ft_dprintf(g_log, "found heredoc: %s\n", current_delimiter->content);
		if (!process_heredoc(current_delimiter, i))
			return (0);
		current_delimiter = current_delimiter->next;
		i++;
		data->heredoc_count++;
	}	
	ft_dprintf(data->log, "printing tokens list\n");
	print_tokens_list(data->log, tokens);
	return (1);
}

