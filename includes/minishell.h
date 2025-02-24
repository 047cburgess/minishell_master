#ifndef MINISHELL_H
# define MINISHELL_H

# define BOLD "\033[1m"
# define PINK "\e[35m"
# define RESET "\033[0m"
# define PROMPT BOLD PINK"Welcome 🌊🦦 >$ "RESET

# define SUCCESS 1
# define FAILURE 0
# define TRUE 1
# define FALSE 0

# define WORD 0
# define RD_HEREDOC 1
# define RD_APPEND 2
# define RD_IN 3
# define RD_OUT 4
# define PIPE 5

# define MAX_OPERATOR_LEN 2

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <signal.h>
# include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>	
#include <fcntl.h>
# include "libft.h"

typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env	*next;
} t_env;

typedef struct s_token
{
	char 		*content;
	int			type;
	struct s_token *next;
} t_token;

typedef struct s_data 
{
	t_token		*tokens_list;
	t_list	*map_list;
	int	command_count;
	char 		**bash_env;
	t_env		*env;
	char	**paths;
	int	log;
} t_data;


// ------ EXECUTION ----- //

char	*get_command(t_token *list);
int	launch_solo_command(t_data *data);
int	type_is_redirection(int type);
char	*get_command_path(t_data *data, char **directories, char *command);
char	**get_av(t_token *tokens, int ac);
int	get_ac(t_token *command_list, t_data *data);
int	handle_redirections(t_data *data, int *in_out);
int	is_builtin(char *command);

// ------ SIGNALS ----- //
//signals.c
void	init_signals(struct sigaction *act);

// ------ PARSING ----- //
// parsing.c

int		handle_expansions(t_data *data, t_env *env);
int		set_environment(char **envp, t_data *data);
int	handle_input(char *line, t_data *data);
void	print_str_array(char **array);
int		unclosed_quote_detected(char *input);
int		is_quote(char c);
int	get_command_count(t_token *list);

// ----- TOKEN_SYNTAX ----- //
int	type_is_redirection(int type);
int	check_pipe_syntax(t_token *tokens);
int	check_valid_redirections(t_token *head);
int	check_token_syntax(t_token *tokens);

// ------ TOKENS ----- //
int		tokenise(char *line, t_data *data);
t_token *new_token_node(char *content);
void	token_add_back(t_token **tokens, t_token *new);
void	token_del_node(t_token *tokens_list, void (*del)(void *));
void	token_lst_clear(t_token **tokens_list, void (*del)(void *));
t_list	*ft_lst_map(t_list *lst, char *(*f)(char *), void (*del)(void *));
void 	print_tokens_list(t_token *tokens_list);
int		is_operator(char c);
int 	ft_mapping(t_data *data, t_list *cutting);
void 	print_map(t_list *map_list);
t_token	*token_lst_last(t_token *head);
int		is_operator(char c);
char 	*expand_token(t_env *env, char *content);
int	expansion_needed(char *content);


// ------ BUILT IN ----- //
int		ft_echo(char **args);
int		ft_pwd(void);
int		ft_cd(char **args);

// ------ BUILT IN HELPERS ----- //
int	execute_builtin(char **av, t_data *data);
int 	count_ac(char **args);
int		count_strings(char **array);

// ----- ENV SET UP ----- //
int		set_environment(char **envp, t_data *data);
void	print_env_list(t_env *env_head);
int		set_environment(char **bash_env, t_data *data);
t_env	*env_node_new(char *key, char *value);
char	*get_key(char *str);
char	*get_value(char *str);
void	env_delone(t_env *node);
void	env_list_clear(t_env **env_list);
t_env	*env_last(t_env *env_head);
void	env_add_back(t_env **env_head, t_env *new_node);
t_env	*env_to_list(char **bash_env);
char	**env_to_array(t_env *env_head);
char	*ft_getenv(t_env *env, char *key);
char	**get_split_paths(t_data *data);

// ----- MINISHELL SHUT DOWN ----- //
void	shut_down_minishell(t_data *data);

// ----- MISC UTILS ----- //
char	**duplicate_str_array(char **start_env);
void	free_str_array(char **array, int size);
void	new_log_timestamp(int fd, char *message);

#endif
