#include "minishell.h"


int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

// to check at school -- seems to not exit sometimes and exits other times.
// Priority 1: too many arguments
// Priority 2: not numeric
// returns always between 0 and 255 -> cast to unsigned int for now - should be 8 bit max so check correct type
int	ft_exit(char **av, t_data *data)
{
	int	ac;

	ac = count_ac(av);
	ft_dprintf(2, "exit\n");
	if (ac > 2)
	{
		ft_dprintf(2, "minishell: exit: too many arguments\n");
		return (1);
	}
	if (ac == 1)
	{
		if (g_signal != 0)
			data->status = g_signal + 128;
		ft_dprintf(g_log, "FT_EXIT: data->status is %i\n", data->status);
		return (data->status);
	}
	else 
	{
		if (!is_numeric(av[1]))
		{
			ft_dprintf(2, "minishell: %s: numeric argument required\n", av[1]);
			return (2);
		}
		return ((unsigned int)ft_atoi(av[1]));
	}
	return (0);
}
