#include "minishell.h"

int	get_heredoc_count(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (token->type == RD_HEREDOC)
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

int	process_heredoc(t_token *delimiter, int id)
{
	char	buffer[4097];
	char	*file_name;
	char	*id;

	// SET THE FILE NAME:w
	//
	ft_bzero(buffer, sizeof(buffer));
	id = ft_itoa(id);
	strlcat(buffer, "/tmp/mini_heredoc_", sizeof(buffer));
	strlcat(buffer), id, sizeof(buffer));
	file_name = ft_strdup(buffer);

	// CREATE & OPEN THE FILE
	//
	// READ AND WRITE INTO IT UNTIL YOU GET TO THE DELIMITER OR 
}

int	handle_heredocs(t_data *data, t_token *tokens)
{
	int	heredoc_count;
	char 	heredoc_path[4097];
	t_token 	*current_delimiter;
	int	i;

	ft_bzero(heredoc_path, 4097);
	heredoc_count = get_heredoc_count(tokens);
	if (heredoc_count == 0)
		return (1);
	i = 0;
	while (i < heredoc_count)
	{
		current_delimiter = get_next_heredoc_delimiter(tokens);
	}
	
}
