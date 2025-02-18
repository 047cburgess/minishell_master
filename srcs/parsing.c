#include "minishell.h"

int	execute_builtin(t_command *cmd);

// just for testing basic input for builtins, no quotes, no pipes, no redirections


int	is_operator(char c)
{
	return ((c == '|') || (c == '<') || (c == '>'));
}

t_list	*get_quoted_token(char *start, char quote)
{
	t_list	*new_token = NULL;
	char	*new_content = NULL;
	int i = 1;

	while (start[i] != quote)
		i++;
	while (start[i] && start[i] != ' ' && !is_operator(start[i]))
		i++;
	new_content = ft_substr(start, 0, i);
	new_token = ft_lstnew(new_content);
	return (new_token);
}

t_list	*get_unquoted_token(char *start)
{
	char	*ptr = start;
	int len;
	t_list	*new_token = NULL;
	char	*new_content = NULL;
	int i = 0;

	while (*ptr && !is_operator(*ptr) && *ptr != ' ')
	{
		if (is_quote(*ptr))
		{
			ptr = ft_strchr(ptr, *ptr) + 1;
		}
		else 
			ptr++;
		i++;
	}
	len = ptr - start;
	new_content = ft_substr(start, 0, len);
	new_token = ft_lstnew(new_content);
	return (new_token);
}


t_list	*get_operator(char *start)
{
	char	*content;
	t_list	*token;
	
	if (ft_strncmp(start, "<<", 2) == 0)
		content = ft_strdup("<<");
	else if (ft_strncmp(start, ">>", 2) == 0)
		content = ft_strdup(">>");
	else if (*start == '|')
       		content = ft_strdup("|");
	else if (*start == '<')
		content = ft_strdup("<");
	else
		content = ft_strdup(">");
	token = ft_lstnew(content);
	return (token);
}
// get key = delimited by: $, spaces, quotes, end of string
// get value = ft_getenv
int	expansion_needed(char *content)
{
	char	*new_line = NULL;
	char	*var = NULL;
	char	*ptr = content;

	if  (ft_strchr(content, '$') == NULL)
		return (FALSE);
	if (ft_strcmp(content, "$") == 0)
		return (FALSE);
	while(*ptr)
	{
		if (*ptr == '\'')
			ptr = ft_strchr(ptr + 1, '\'') + 1;
		else if (*ptr == '\"')
		{
			ptr++;
			while (*ptr && *ptr != '\"')
			{
				if (*ptr == '$' && *(ptr + 1) != '\"')
					key = ft_getkey
					var = ft_getenv(key)
					ft_strjoin(line, var);		
				ptr++;
			}
		}
		else if (*ptr == '$' && *(ptr + 1))
			return (TRUE);
		ptr++;
	}
	return (FALSE);
}

int	expansion_needed(char *content)
{
	char	*ptr = content;

	if  (ft_strchr(content, '$') == NULL)
		return (FALSE);
	if (ft_strcmp(content, "$") == 0)
		return (FALSE);
	while(*ptr)
	{
		if (*ptr == '\'')
			ptr = ft_strchr(ptr + 1, '\'') + 1;
		else if (*ptr == '\"')
		{
			ptr++;
			while (*ptr && *ptr != '\"')
			{
				if (*ptr == '$' && *(ptr + 1) != '\"')
					return (TRUE);
				ptr++;
			}
		}
		else if (*ptr == '$' && *(ptr + 1))
			return (TRUE);
		ptr++;
	}
	return (FALSE);
}

char	*first_expand_outside_quotes(char *line)
{
	char	*new_line = NULL;
	i = 0;

	while (line[i] != '\0')
	{
		if (is_quote(line[i]))
		{
			while (



}




/*
void	ft_expand(t_list *tokens)
{
	char	*new_content = NULL;
	int	i = 0;

	while (tokens)
	{
		if (expansion_needed(tokens->content))
		{
			new_content = get_expanded_content(tokens->content);
			free(tokens->content);
			tokens->content = new_content;
		}	
		tokens = tokens->next;
	}
}*/

int	parse_and_execute(char *line, t_data *data)
{
	(void)*data;
	t_list	*token_list = NULL;
	t_list	*new_token = NULL;

	if (unclosed_quote_detected(line))
		return (FAILURE);

	// GET TOKEN LIST
	int i = 0;
	while (line[i] != '\0')
	{
		while (isspace(line[i]))
			i++;
		if (is_operator(line[i]))
		{
			new_token = get_operator(&line[i]);
			ft_lstadd_back(&token_list, new_token);
			i += ft_strlen(new_token->content);
		}
		else if (is_quote(line[i]))
		{
			new_token = get_quoted_token(&line[i], line[i]);
			ft_lstadd_back(&token_list, new_token);
			i += ft_strlen(new_token->content);
		}
		else if (!is_quote(line[i]))
		{
			new_token = get_unquoted_token(&line[i]);
			ft_lstadd_back(&token_list, new_token);
			i += ft_strlen(new_token->content);
		}
	}
	//ft_expand(&token_list);
	while (token_list)
	{
		printf("[%s] ", (char *)token_list->content);
		if (expansion_needed(token_list->content))
			printf("\tExpansion needed!\n");
		else
			printf("\tExpansion not needed!\n");
		token_list = token_list->next;
	}
	printf("\n");
	return (0);
}



int	handle_input2(char *line, t_data *data)
{
	t_command	*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	cmd->argv = ft_split(line, ' ');
	cmd->cmd = cmd->argv[0];
	cmd->env = data->bash_env;
	data->command = cmd;
	//print_str_array(cmd->argv);
	//print_str_array(cmd->env);
	execute_builtin(data->command);
	// Clean up the memory
	free_str_array(cmd->argv, count_strings(cmd->argv));
	free(cmd);
	return (SUCCESS);
}

int	execute_builtin(t_command *cmd)
{
	if (ft_strcmp(cmd->cmd, "echo") == 0)
		ft_echo(&cmd->argv[1]);
	else if (ft_strcmp(cmd->cmd, "cd") == 0)
		ft_cd(&cmd->argv[1]);
	else if (ft_strcmp(cmd->cmd, "pwd") == 0)
		ft_pwd();
	else
		printf("Not a built in command\n");
	return (SUCCESS);
}
