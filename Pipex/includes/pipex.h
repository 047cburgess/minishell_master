/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caburges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:32:01 by caburges          #+#    #+#             */
/*   Updated: 2025/01/28 14:11:45 by caburges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/types.h>
# include <sys/wait.h>
# include "libft.h"
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_pipe
{
	int		pipe_fd[2];
	int		in_fd;
	int		out_fd;
	pid_t	pid1;
	pid_t	pid2;
	char	**args;
	char	*paths;
	char	**split_paths;
	char	*path;
}	t_pipe;

int		check_ac(int ac);
int		check_envp(char **envp);
void	init_struct(t_pipe *data);
void	set_up_pipe(int *pipe_fd);
void	run_command_1(t_pipe *pipe, char **av, char **envp);
void	run_command_2(t_pipe *pipe, char **av, char **envp);
void	execute_command(t_pipe *pipe, char *cmd, char **envp);

// CLEAN UP SHIT
void	clean_up_exit(t_pipe *data, int status);
void	free_split(char **array);
void	close_pipe(t_pipe *data);
void	ft_free(char **var);
void	exit_malloc(t_pipe *data);

#endif
