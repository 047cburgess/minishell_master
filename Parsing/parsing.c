#include "minishell.h"
#include "ft_dprintf.h"


// Sets the non interactive signals for the start of the function
// Signals get reset to interactive in the main when it's needed
// Point of control between lexer, parser and executor
int	handle_input(char *line, t_data *data)
{
	new_log_timestamp(data->log, line);
	set_noninteractive_signals();
	
	if (!minishell_lexer(data, line))
		return (FAILURE);

	if (!minishell_parser(data))
	{
		// function to unlink heredocs
		token_lst_clear(&data->tokens_list, free);
		return (FAILURE);
	}
	minishell_executor(data, data->command_count, data->command_list);
	return (SUCCESS);
}

// Point of control for the parsing process
// Prints any error messages
// Sets the relevant data status
// Frees what it allocated when necessary
// If success, tokens and commands guaranteed to be non-null and correct
int	minishell_parser(t_data *data)
{
	if (check_token_syntax(data->tokens_list) == FAILURE)
	{
		data->status = 2;
		return (FAILURE);
	}
	data->command_count = get_command_count(data->tokens_list);
	if (handle_expansions_in_tokens(data) == FAILURE)
	{
		data->status = 1; // Will always be a malloc failure
		return (FAILURE);
	}
	if (handle_heredocs(data, data->tokens_list) == FAILURE)
	{
		delete_heredocs_files(data, data->tokens_list);
		if (catch_signals_for_data_status(data))
		{
			ft_dprintf(g_log, "ctl c found (if catch signals for dataa status)\n");
			return (FAILURE);
		}
		perror("heredocs: fail");
		data->status = 1;
		return (FAILURE);
	}
	if (prep_command_tables(data, data->tokens_list) == FAILURE)
	{
		perror("prep_command_tables: malloc failure");
		data->status = 1;
		return(FAILURE);
	}
	return (SUCCESS);
}

int	get_command_count(t_token *list)
{
	int	pipe_count;

	pipe_count = 0;
	while (list != NULL)
	{
		if (list->type == PIPE)
			pipe_count++;
		list = list->next;
	}
	return (pipe_count + 1);
}
