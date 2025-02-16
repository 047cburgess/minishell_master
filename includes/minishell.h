#ifndef MINISHELL_H
# define MINISHELL_H

# define BOLD "\033[1m"
# define RESET "\033[0m"
# define PROMPT BOLD"Welcome 🌊🦦 >$ "RESET

# define SUCCESS 1
# define FAILURE 0

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <signal.h>
# include <stdbool.h>
# include "libft.h"

// ----- STRUCTURES NEEDED ----- //
// PARSING TOKENS
// 	-> str
// 	-> type
// 	-> next 
//
// COMMANDS
// 	-> argv
//	-> infile
//	-> outfile

typedef enum e_type
{
	command,
	pipe,
	rd_in,
	rd_out,
	heredoc,
	append,
} e_type;

typedef struct s_command
{
	char	**argv;
	char	*cmd;
	char	*exec_path;
	char	**env;
} t_command;

typedef struct s_data 
{
	t_command *command;
	char	**env;
} t_data;

// ------ SIGNALS ----- //
//signals.c
void	init_signals(struct sigaction *act);


// ------ PARSING ----- //
// parsing.c
int	handle_input(char *line, t_data *data);
int	set_environment(char **envp, t_data *data);
void	print_str_array(char **array);


// ------ BUILT IN ----- //
int	ft_echo(char **args);
int    ft_pwd(void);
int	ft_cd(char **args);

// ------ BUILT IN HELPERS ----- //
int 	count_ac(char **args);

#endif
