#include "minishell.h"

int	get_child_exit_status(int status)
{
	int	exit_signal;

	exit_signal= 0;
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{	
		exit_signal = WTERMSIG(status);
		return (128 + exit_signal);
	}
	else
		return (1);
}
