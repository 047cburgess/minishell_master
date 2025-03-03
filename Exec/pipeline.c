#include "minishell.h"

// one pipe shared between 2 commands
//
// 1st command  pipe1[]
// middle command
// last command
int	launch_first_child_pipe(t_data *data, t_command *cmd, int *pipe_fds);
int	launch_middle_child_pipe(t_data *data, t_command *cmd, int* pipe_fds, t_command *prev);
int	launch_last_child_pipe(t_data *data, t_command *cmd, int *pipe_fds);

int	launch_pipeline(t_data *data, t_command *commands, int num_cmds)
{
	int	pipe_fds[2];
	t_command	*current;
	t_command	*prev;
	int	i;

	current = commands;
	i = 0;
	while (i < num_cmds)
	{
		if (i < num_cmds - 1)
			pipe(pipe_fds);
		if (i == 0)
			launch_first_child_pipe(data, current, pipe_fds);
		else if (i < num_cmds - 1)
			launch_middle_child_pipe(data, current, pipe_fds, prev);
		else
			launch_last_child_pipe(data, current, pipe_fds);
		prev = current;
		current = current->next;
		i++;
	}

	// WAIT FOR ALL
	i = 0;
	while (i < num_cmds)
	{
		if (i == num_cmds - 1)
			waitpid(commands->pid, &data->status, 0);
		else
			waitpid(commands->pid, NULL, 0);
		commands = commands->next;
		i++;
	}
	data->status = get_child_exit_status(data->status);
	return (0);
}

int	launch_first_child_pipe(t_data *data, t_command *cmd, int *pipe_fds)
{
	cmd->pipe_fds[0] = pipe_fds[0]; // unused end of pipe
	cmd->pipe_fds[1] = pipe_fds[1]; // represents the pipe it will WRITE INTO
	cmd->pid = fork();
	if (cmd->pid == -1)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		data->status = 1;
		return (1);
	}
	if (cmd->pid == 0)
	{
		close(cmd->pipe_fds[0]);
		cmd->fds[1] = cmd->pipe_fds[1];
		dup2(cmd->fds[1], STDOUT_FILENO);
		close(cmd->fds[1]);
		handle_redirections(data, cmd, cmd->fds);
		ft_dprintf(data->log, PINK"%s: in:%i, out:%i\n", cmd->av[0], cmd->fds[0], cmd->fds[1]);
		ft_dprintf(data->log, PINK"%s: in:%i, out:%i\n", cmd->av[0], cmd->fds[0], cmd->pipe_fds[1]);
		if (cmd->ac == 0 && cmd->error == 0)
			cmd->error = ER_NO_CMD;
		set_command_path(data, cmd->path, cmd->av[0], cmd);
		check_access(cmd->path, data, cmd);
		print_errors_and_exit(data, cmd);
		execve(cmd->path, cmd->av, data->env_array);
		ft_dprintf(2, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		close_fds(cmd);
		clean_up_exit(data, errno, NULL);
	}
	return (0);
}

int	launch_middle_child_pipe(t_data *data, t_command *cmd, int* pipe_fds, t_command *prev)
{
	cmd->pipe_fds[0] = pipe_fds[0]; // represents the pipe it will NOT NEED
	cmd->pipe_fds[1] = pipe_fds[1]; // represents the pipe it will WRITE INTO
	cmd->pid = fork();
	if (cmd->pid == -1)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		data->status = 1;
		return (1);
	}
	if (cmd->pid == 0)
	{
		close(cmd->pipe_fds[0]);
		close(prev->pipe_fds[1]);
		cmd->fds[0] = prev->pipe_fds[0];
		cmd->fds[1] = pipe_fds[1];
		if (dup2(cmd->fds[0], STDIN_FILENO) == -1)
			ft_dprintf(data->log, "%s: failed to dup stdin to prev pipe\n", cmd->av[0]);
		close(cmd->fds[0]);
		dup2(cmd->fds[1], STDOUT_FILENO);
		close(cmd->fds[1]);
		handle_redirections(data, cmd, cmd->fds);
		if (cmd->ac == 0 && cmd->error == 0)
			cmd->error = ER_NO_CMD;
		set_command_path(data, cmd->path, cmd->av[0], cmd);
		check_access(cmd->path, data, cmd);
		print_errors_and_exit(data, cmd);
		execve(cmd->path, cmd->av, data->env_array);
		ft_dprintf(2, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		close_fds(cmd);
		clean_up_exit(data, errno, NULL);
	}
	close(prev->pipe_fds[0]); // closes read end of previous pipe as never needed again
	close(prev->pipe_fds[1]); // closes read end of previous pipe as never needed again
	return (0);
}

int	launch_last_child_pipe(t_data *data, t_command *cmd, int *pipe_fds)
{
	// PARENT
	cmd->pipe_fds[0] = pipe_fds[0];
	cmd->pid = fork();
	if (cmd->pid == -1)
	{
		ft_dprintf(2, "minishell: %s\n", strerror(errno));
		data->status = 1;
		return (1);
	}
	if (cmd->pid == 0)
	{
		cmd->fds[0] = pipe_fds[0];
		close(pipe_fds[1]);
		ft_dprintf(data->log, "-----last child pipe-----\n");
		if (dup2(cmd->fds[0], STDIN_FILENO) == -1)
			ft_dprintf(data->log, "%s: failed to dup stdin to prev pipe\n", cmd->av[0]);
		close(cmd->fds[0]);
		handle_redirections(data, cmd, cmd->fds);
		if (cmd->ac == 0 && cmd->error == 0)
			cmd->error = ER_NO_CMD;
		set_command_path(data, cmd->path, cmd->av[0], cmd);
		check_access(cmd->path, data, cmd);
		print_errors_and_exit(data, cmd);
		execve(cmd->path, cmd->av, data->env_array);
		ft_dprintf(2, "minishell: %s: %s\n", cmd->av[0], strerror(errno));
		close_fds(cmd);
		clean_up_exit(data, errno, NULL);
	}
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	return (0);
}
