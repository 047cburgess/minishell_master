#include "minishell.h"

// Point of control for the lexing process.
// Prints any error messages
// Sets the relevant data status
// Frees what it allocated if necessary
// If success, tokens are guaranteed to be non_null
int	line_is_whitespace(char *line);

int	minishell_lexer(t_data *data, char *line)
{
	if (line_is_whitespace(line)) // doesn't update the data status -> correct
		return (FAILURE);
	if (unclosed_quote_detected(line))
	{
		ft_dprintf(2, "minishell: unclosed quote detected\n");
		data->status = 1;
		return (FAILURE);
	}
	if (tokenise(line, data) == FAILURE)
	{
		perror("tokenise: malloc failure");
		data->status = 1;
		return (FAILURE);
	}
	return (SUCCESS);
}

// Checks for space, tab and new line characters as per bash MAN
int	line_is_whitespace(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
			return (0);
		i++;
	}
	return (1);
}
