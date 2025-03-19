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

// reads user input from "<<" and writes into temporary file
void	read_and_write(char **line, t_token *delimiter, int fd, t_data *data)
{
	char	*expanded_line;

	while (g_signal == 0)
	{
		*line = readline("> ");
		if (*line == NULL)
		{
			if (g_signal == 0)
				ft_dprintf(2, ER_HEREDOC_MSG, delimiter->content);
			break ;
		}
		else if (ft_strcmp(*line, delimiter->content) == 0)
			break ;
		if (delimiter->exp)
		{
			expanded_line = get_hd_line(*line, data);
			if (!expanded_line)
				break ;
			write(fd, expanded_line, ft_strlen(expanded_line));
			ft_free((void *)&expanded_line);
		}
		else
			write(fd, *line, ft_strlen(*line));
		put_newl_and_free(fd, line);
	}
}

// stop reading when line contains only delimiter OR ctld / ctlc is received (null)
int	write_into_heredoc(int fd, t_token *delimiter, t_data *data)
{
	char	*line;
	int		stdin_dup;

	signal(SIGINT, heredoc);
	stdin_dup = dup(STDIN_FILENO);
	if (stdin_dup == -1)
		return (signal(SIGINT, SIG_IGN), 0);
	line = NULL;
	read_and_write(&line, delimiter, fd, data);
	ft_free((void *)&line);
	dup2(stdin_dup, STDIN_FILENO);
	close(stdin_dup);
	signal(SIGINT, SIG_IGN);
	if (g_signal != 0 || data->expansion_status != 0)
		return (0);
	return (1);
}

// generates a unique filename for a heredoc
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
	if (*file_name == NULL)
		return (0);
	return (1);
}

int	process_heredoc(t_token *delimiter, int id, t_data *data)
{
	char	*file_name;
	int		fd;

	file_name = NULL;
	if (set_file_name(&file_name, id) == FAILURE)
		return (0);
	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (ft_free((void *)&file_name), 0);
	if (!write_into_heredoc(fd, delimiter, data))
	{
		unlink(file_name);
		ft_free((void *)&file_name);
		ft_close(&fd);
		return (0);
	}
	ft_close(&fd);
	free(delimiter->content);
	delimiter->content = file_name;
	return (1);
}

// creates all heredocs identified in the lexing process
int	handle_heredocs(t_data *data, t_token *tokens)
{
	int		heredoc_count;
	t_token	*current_delimiter;
	int		i;

	heredoc_count = get_heredoc_count(tokens);
	if (heredoc_count == 0)
		return (1);
	current_delimiter = tokens;
	i = 0;
	while (i < heredoc_count)
	{
		current_delimiter = get_next_heredoc_delimiter(current_delimiter);
		if (!process_heredoc(current_delimiter, i, data))
			return (0);
		current_delimiter = current_delimiter->next;
		i++;
		data->heredoc_count++;
	}
	return (1);
}
