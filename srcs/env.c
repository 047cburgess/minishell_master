
#include "minishell.h"

void	ft_env(t_data *data)
{
	printf("--ENV--\n");
	print_env_list(data->env);
}