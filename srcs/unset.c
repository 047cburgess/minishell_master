
#include "minishell.h"

void ft_unset(char **av, t_data *data)
{
	int 	i;
	t_env	*current_env;
	t_env 	*temp;
	t_env 	*prev;

	if (!av[1])
		return ;
	i = 1;
	while (av[i])
	{
		current_env = data->env;
		prev = NULL;
		while (current_env)
		{
			if (ft_strcmp(current_env->key, av[i]) == 0)
			{
				temp = current_env;
				if (prev == NULL)
					data->env = current_env->next;
				else
					prev->next = current_env->next;
				env_delone(temp);
				break;
			}
			prev = current_env;
			current_env = current_env->next;
		}
		i++;
	}
}
