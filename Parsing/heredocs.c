#include "minishell.h"

int	get_heredoc_count(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == RD_HEREDOC)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

t_token	*get_next_heredoc_delimiter(t_token *tokens)
{
	while (tokens && tokens->type != RD_HEREDOC)
		tokens = tokens->next;

	if (tokens == NULL)
		return (NULL);
	else 
		return (tokens->next);
}

// Get the number of heredocs to manage
// Generate the unique file names
// run the heredoc
// 	
// replace the delimiter by the filename

// stop reading when line contains only delimiter OR ctld is received (null)
int	write_into_heredoc(int fd, char *delimiter)
{
	signal(SIGINT, heredoc);
	ft_dprintf(g_log, "FUNCTION: WRITE INTO HEREDOC\n");
	char	*line;
	int stdin_dup = dup(STDIN_FILENO);
	if (stdin_dup == -1)
	{
		signal(SIGINT, SIG_IGN);
		return (0);
	}
	line = NULL;
	ft_dprintf(g_log, "delimiter: %s\n", delimiter);
	while (g_signal == 0)
	{
		ft_dprintf(g_log, "g signal not at zero %i\n", g_signal);
		ft_dprintf(g_log, "writing the heredoc prompt\n");
		//write(1, ">", 1);
		line = readline("> ");
		ft_dprintf(g_log, "line: %s", line);
		if (!line)
		{
			ft_dprintf(g_log, "ended with Ctrl-D OR Ctrl-C\n");
			break;
		}
		else if (ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
		{
			ft_dprintf(g_log, "delimiter found\n");
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		ft_free((void *)&line);
	}
	// flush gnl
	//get_next_line(-1);
	ft_free((void *)&line);
	ft_dprintf(g_log, "preparin to close stdindup\n");
	dup2(stdin_dup, STDIN_FILENO);
	close(stdin_dup);
	signal(SIGINT, SIG_IGN);
	if (g_signal != 0)
		return (0);
	return (1);
}

int	process_heredoc(t_token *delimiter, int id)
{
	char	buffer[4097];
	char	*file_name;
	char	*hd_id;
	int	fd;

	ft_dprintf(g_log, "FUNCTION: PROCESS HEREDOC\n");
	ft_bzero(buffer, sizeof(buffer));

	// SET THE FILE NAME
	hd_id = ft_itoa(id);
	if (!hd_id)
		return (0);
	ft_strlcat(buffer, ".heredoc", sizeof(buffer));
	ft_strlcat(buffer, hd_id, sizeof(buffer));
	free(hd_id);
	file_name = ft_strdup(buffer);
	if (!file_name)
		return (0);

	ft_dprintf(g_log, "filename1: %s\n", file_name);

	// CREATE & OPEN THE FILE
	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_free((void*)&file_name);
		return (0);
	}

	// READ AND WRITE INTO IT UNTIL YOU GET TO THE DELIMITER OR EOF
	if (!write_into_heredoc(fd, delimiter->content))
	{
		ft_dprintf(g_log, "unlinking '%s'\n", file_name);
		unlink(file_name);
		ft_free((void*)&file_name);
		close(fd);
		return (0);
	}

	// CLOSE THE TEMP FILE
	close(fd);
	ft_dprintf(g_log, "closed heredoc %i\n", id);

	// REPLACE THE TOKEN WITH THE FILENAME
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

int delete_heredocs_files(t_data *data, t_token *tokens)
{
	int i;
	t_token *current_delimiter;
	ft_dprintf(g_log, "TOKENS BEFORE DELETE HEREDOCS FILES\n");
	print_tokens_list(data->log, tokens);
	current_delimiter = tokens;
	i = 0;
	while (i < data->heredoc_count)
	{
		current_delimiter = get_next_heredoc_delimiter(current_delimiter);
		ft_dprintf(g_log, "curr delimiter content: %s\n", current_delimiter->content);
		unlink(current_delimiter->content);
		i++;
		current_delimiter = current_delimiter->next;
	}
	ft_dprintf(g_log, "RESET HEREDOC COUNT TO 0\n");
	data->heredoc_count = 0;
	return (1);
}
