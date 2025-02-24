#include "minishell.h"

// This function creates a copy of the starting environment strings array,
// and saves it into the program data for minishell new env

void	free_str_array(char **array, int size);
void	print_str_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		printf("%s\n", array[i]);
		i++;
	}
}

// ARCHIVE: This was the function to get our starting environment in the string array format
int	set_environment_old(char **start_env, t_data *data)
{
	ft_bzero(data, sizeof(*data));
	data->bash_env = duplicate_str_array(start_env);
	if (data->env == NULL)
		return (FAILURE);
	else
		return (SUCCESS);
}

// This function frees an array of strings according to number of strings in it
void	free_str_array(char **array, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

// This function duplicate a NULL-terminated an array of strings
char	**duplicate_str_array(char **start_env)
{
	char	**duplicate_array;
	int	string_count;
	int	i;

	string_count = count_strings(start_env);
	duplicate_array = ft_calloc((string_count + 1), sizeof(char *));
	if (!duplicate_array)
		return (NULL);
	i = 0;
	while (i < string_count)
	{
		duplicate_array[i] = ft_strdup(start_env[i]);
		if (!duplicate_array[i])
		{
			free_str_array(duplicate_array, i);
			return (FAILURE);
		}
		i++;
	}
	return (duplicate_array);
}

// Counts the number of strings in a string array
int	count_strings(char **array)
{
	int	count;

	if (array == NULL)
		return (0);
	count = 0;
	while (array[count] != NULL)
		count++;
	return (count);
}
