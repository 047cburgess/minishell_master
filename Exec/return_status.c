#include "minishell.h"

int	get_child_exit_status(int status)
{
	int	exit_signal;

	exit_signal= 0;

	// If it exited normally, retrieve its exit code
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	// If it exited by a signal, return the signal num + 128
	else if (WIFSIGNALED(status))
	{	
		exit_signal = WTERMSIG(status);
		return (128 + exit_signal);
	}
	else
		return (1);
}
