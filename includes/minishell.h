#ifndef MINISHELL_H
# define MINISHELL_H

# define BOLD "\033[1m"
# define RESET "\033[0m"
# define PROMPT BOLD"Welcome 🌊🦦 >$ "RESET

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <signal.h>
# include <stdbool.h>
# include "libft.h"

// ------ SIGNALS ----- //
//signals.c
void	init_signals(struct sigaction *act);


// ------ BUILT IN ----- //
int	ft_echo(char **args);
int    print_pwd(void);
int	ft_cd(char **args);

// ------ BUILT IN HELPERS ----- //
int 	count_ac(char **args);

#endif
