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

#define S_IFMT 0170000
#define S_IFDIR 0040000

# define MAX_OPERATOR_LEN 2

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <signal.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>	
# include <fcntl.h>
# include "libft.h"
# include "ft_dprintf.h"
# include <errno.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	int				no_value;
	struct s_env	*next;
} t_env;

typedef struct s_token
{
	char 			*content;
	int				type;
	struct s_token *next;
} t_token;

typedef struct s_command
{
	char			**av;
	char			**env;
	int				ac;
	int				fds[2];
	t_token 		*tokens;
	char			*path;
	char			**path_dirs;
	pid_t			pid;
	int	error;
	struct s_command *next;
} t_command;

typedef struct s_data 
{
	t_token		*tokens_list;
	t_list		*map_list;
	t_command	*command_list;
	int			command_count;
	char 		**bash_env;
	t_env		*env;
	t_env		*export;
	int			log;
	int 		status;
} t_data;

// ------ EXECUTION ----- //

int		execute_solo_child(t_data *data, t_command *cmd);
char	*get_command(t_token *list);
int		launch_solo_command(t_data *data);
int		type_is_redirection(int type);
char	*get_command_path(t_data *data, char *command);
char	**get_av(t_token *tokens, int ac);
int		get_ac(t_token *command_list);
int		handle_redirections(t_data *data, t_command *cmd, int *in_out);
int	is_redirection_in(int type);
int	handle_redirection_in(t_data *data, t_command *cmd, int *in_out, t_token *token);
int	handle_redirection_out(t_data *data, t_command *cmd, int *in_out, t_token *token);
int		is_builtin(char **av);
int	check_access(char *full_path, t_data *data, t_command *cmd);

// ------ COMMAND TABLE ------ //

t_command	*new_command_table(t_token *tokens, t_data *data);
void		command_add_back(t_command **head, t_command *new);
void		command_del_node(t_command *cmd);
void		command_lst_clear(t_command **head);
t_command	*command_lst_last(t_command *head);
void		print_command_list(t_command *head);

// ------ SIGNALS ----- //
//signals.c
void	init_signals(struct sigaction *act);
int		get_child_exit_status(int status);

// ------ PARSING ----- //
// parsing.c

int		set_environment(char **envp, t_data *data);
int		handle_input(char *line, t_data *data);
void	print_str_array(char **array);
int		unclosed_quote_detected(char *input);
int		is_quote(char c);
int		get_command_count(t_token *list);

// ----- TOKEN_SYNTAX ----- //
int		type_is_redirection(int type);
int		check_pipe_syntax(t_token *tokens);
int		check_valid_redirections(t_token *head);
int		check_token_syntax(t_token *tokens);

// ------ TOKENS ----- //
int		tokenise(char *line, t_data *data);
t_token *new_token_node(char *content);
void	token_add_back(t_token **tokens, t_token *new);
void	token_del_node(t_token *tokens_list, void (*del)(void *));
void	token_lst_clear(t_token **tokens_list, void (*del)(void *));
t_list	*ft_lst_map(t_list *lst, char *(*f)(char *), void (*del)(void *));
void 	print_tokens_list(int fd, t_token *tokens_list);
int		is_operator(char c);
int 	ft_mapping(t_data *data, t_list *cutting);
void 	print_map(t_list *map_list);
t_token	*token_lst_last(t_token *head);
int		is_operator(char c);
int		expansion_needed(char *content);

// ------ EXPANSIONS ----- //
char 	*find_key(char *line, int i);
char	*convert_expansion(t_data *data, char *line, int *i);
void	handle_double_quotes(t_data *data, t_list **cutting, char *line, int *i);
void	handle_simple_text(t_list **cutting, char *line, int *i);
void	handle_simple_quotes(t_list **cutting, char *line, int *i);
char 	*expansion_line(t_data *data, char *line);
char 	*expand_token(t_data *data, char *content);
int 	handle_expansions(t_data *data);

// ------ BUILT IN ----- //
int		ft_echo(char **args);
int		ft_pwd(void);
int		ft_cd(char **args);
int		ft_export(char **av, t_data *data);
bool	key_is_valid(char *key);
int 	ft_unset(char **av, t_data *data);
int		ft_env(t_data *data);

// ------ BUILT IN HELPERS ----- //
int		execute_builtin(char **av, t_data *data);
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
int		env_lst_size(t_env *env);

// ----- MINISHELL SHUT DOWN ----- //
void	shut_down_minishell(t_data *data);
void	close_fds(t_command *cmd);
void	clean_up_exit(t_data *data, int exit_status, char *msg);

// ----- MISC UTILS ----- //
char	**duplicate_str_array(char **start_env);
void	free_str_array(char **array, int size);
void	new_log_timestamp(int fd, char *message);
int		count_strings(char **array);

#endif
