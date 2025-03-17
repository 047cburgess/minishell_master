/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsuchon <alsuchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:01:40 by caburges          #+#    #+#             */
/*   Updated: 2025/03/17 18:30:50 by alsuchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define BOLD "\001\033[1m\002"
# define PINK "\001\e[35m\002"
# define RESET "\001\033[0m\002"
# define PROMPT "Welcome 🌊🦦 >$ "

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

# define CHILD 1
# define PARENT 0

# define S_IFMT 0170000
# define S_IFDIR 0040000

# define MAX_OPERATOR_LEN 2
# define FULL_PATH_MAX 4097

//--- ERROR CODES ---//
# define ER_IS_DIR 500
# define ER_NO_CMD 501
# define ER_CMD_NOT_FOUND 505
# define ER_NOT_EXECUTABLE 126
# define ER_FAILED_RD 1
# define ER_FORK 503
# define ER_PIPE 504

//--- ERROR MESSAGES ---//
# define ER_MSG_SYNTX_TKN "minishell: syntax error near unexpected token"
# define ER_HEREDOC_MSG "minishell: warning: delimited by EOF (wanted '%s')\n"
# define ER_FT_EXIT_VAL "minishell: exit: %s: numeric argument required\n"
# define ER_FT_EXIT_ARG "minishell: exit: too many arguments\n"
# define ER_OPEN "minishell: %s: %s\n"

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
# include <ctype.h>

extern int	g_signal;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				no_value;
	struct s_env	*next;
}					t_env;

typedef struct s_token
{
	char			*content;
	int				type;
	struct s_token	*next;
}					t_token;

typedef struct s_command
{
	char				**av;
	int					ac;
	int					fds[2];
	t_token				*tokens;
	char				path[FULL_PATH_MAX];
	pid_t				pid;
	int					error;
	struct s_command	*next;
}					t_command;

typedef struct s_data
{
	t_token		*tokens_list;
	t_list		*map_list;
	t_command	*command_list;
	int			command_count;
	char		**bash_env;
	t_env		*env;
	t_env		*export;
	char		**path_dirs;
	char		**env_array;
	int			log;
	int			status;
	t_list		*cutting;
	int			heredoc_count;
	int			expansion_status;
	char		*line;
}			t_data;

t_command	*new_command_table(t_token *tokens);
t_command	*command_lst_last(t_command *head);
t_command	*get_command_tables(t_token *tokens);
// ------ EXECUTION ----- //

int			execute_builtin(char **av, t_data *data, t_command *cmd);
int			launch_builtin(t_data *data, t_command *cmd);
int			execute_solo_child(t_data *data, t_command *cmd);
char		*get_command(t_token *list);
int			launch_solo_command(t_data *data, t_command *command);
int			type_is_redirection(int type);
int			set_cmd_path(t_data *data, char *path, char *com, t_command *cmd);
char		**get_av(t_token *tokens, int ac);
int			get_ac(t_token *command_list);
int			handle_redirections(t_command *cmd, int *in_out);
int			is_redirection_in(int type);
int			is_redirection_out(int type);
int			handle_redirection_in(int *in_out, t_token *token);
int			handle_redirection_out(int *in_out, t_token *token);
int			is_builtin(char **av);
int			check_access(char *full_path, t_command *cmd);
void		clean_job_memory(t_data *data);
int			print_errors_and_exit(t_data *data, t_command *command, int mode);
int			dup_stds(int *std_save);
int			restore_stds(int *std_save);
int			launch_pipeline(t_data *data, t_command *commands, int num_cmds);
int			connect_first_child_pipe(int *fds, t_command *cmd);
int			connect_middle_child_pipe(int *fds, t_command *cmd, t_command *prv);
int			connect_last_child_pipe(t_command *cmd, t_command *prev);
void		close_all_fds(t_data *data);
void		ft_close(int *fd);
void		minishell_executor(t_data *data, int count, t_command *commands);
void		check_no_or_empty_command(t_command *cmd);
int			create_fork(t_command *cmd);
int			create_pipe(t_command *cmd);

// ------ COMMAND TABLE ------ //

int			prep_command_tables(t_data *data, t_token *tokens);
void		command_add_back(t_command **head, t_command *new);
void		command_del_node(t_command *cmd);
void		command_lst_clear(t_command **head);
void		print_command_list(t_command *head);

// ------ SIGNALS ----- //
//signals.c
void		init_interactive_signals(void);
int			get_child_exit_status(int status);
void		init_interactive_signals(void);
void		set_noninteractive_signals(void);
void		restore_signals_for_child(void);
void		heredoc(int signal);
int			catch_signals_for_data_status(t_data *data);

// ------ PARSING ----- //
// parsing.c

int			set_environment(char **envp, t_data *data);
int			handle_input(char *line, t_data *data);
void		print_str_array(char **array);
int			unclosed_quote_detected(char *input);
int			is_quote(char c);
int			get_command_count(t_token *list);
int			minishell_parser(t_data *data);
int			minishell_lexer(t_data *data, char *line);

// ----- TOKEN_SYNTAX ----- //
int			type_is_redirection(int type);
int			check_pipe_syntax(t_token *tokens);
int			check_valid_redirections(t_token *head);
int			check_token_syntax(t_token *tokens);

// ------ TOKENS ----- //
int			tokenise(char *line, t_data *data);
t_token		*new_token_node(char *content);
void		token_add_back(t_token **tokens, t_token *new);
void		token_del_node(t_token *tokens_list, void (*del)(void *));
void		token_lst_clear(t_token **tokens_list, void (*del)(void *));
t_list		*ft_lst_map(t_list *lst, char *(*f)(char *), void (*del)(void *));
int			is_operator(char c);
t_token		*token_lst_last(t_token *head);
int			is_operator(char c);
int			expansion_needed(char *content);
int			handle_operator_token(int *i, char *start, t_token **tokens_list);
int			handle_word_token(int *i, char *start, t_token **tokens_list);
t_token		*get_word_token(char *start);
t_token		*get_operator_token(char *start);
int			get_token_type(char *content);

// ------ EXPANSIONS ----- //
char		*find_key(char *line, int i);
t_list		*convert_var_expansion(t_data *data, char *line, int *i);
int			extract_double_quotes(t_data *data, char *line, int *i);
int			empty_quotes(t_data *data);

int			handle_simple_text(t_data *data, char *line, int *i);
int			handle_simple_quotes(t_data *data, char *line, int *i);
char		*expansion_line(t_data *data, char *line);
char		*expand_token(t_data *data, char *content);
int			handle_expansions_in_tokens(t_data *data);
char		*join_list(t_list **lst);
int			handle_dollar_alone(t_data *data, int *i);
int			handle_exit_extansion(t_data *data, char *line, int *i);
int			handle_expansion(t_data *data, char *line, int *i);
char		*heredoc_delim_tkn(char *line);

// ------ HEREDOC ----- //
int			handle_heredocs(t_data *data, t_token *tokens);
int			delete_heredocs_files(t_data *data, t_token *tokens);
t_token		*get_next_heredoc_delimiter(t_token *tokens);
int			get_heredoc_count(t_token *tokens);

// ------ BUILT IN ----- //
int			ft_echo(char **args);
int			ft_pwd(void);
int			ft_cd(char **av, t_data *data);
int			ft_env(t_data *data);
int			ft_exit(char **av, t_data *data, t_command *cmd);

// ------ Export utils----- //
int			ft_export(char **av, t_data *data);
t_env		*ft_new_node(char *key, char *value);
t_env		*init_new_node(char *key);
bool		key_is_valid(char *key);
t_env		*find_var_in_list(t_env *list, const char *key);
void		bubble_sort_ascii(t_env **env_tab, int size);
int			print_ascii_export(t_data *data);
int			update_or_add_var_env(t_data *data, char *key, char *value);
int			update_or_add_var_export(t_data *data, char *key);
int			add_var_in_export(t_data *data, char *av);
int			add_var_in_env(t_data *data, char *av, char *sign_egal);
t_env		**create_sorted_export_list(t_data *data, int *size);
void		print_sorted_export_list(t_env **export_tab, int size);
void		expansion_no_exist(t_data *data, char *av);

// ------ Unset utils----- //
int			ft_unset(char **av, t_data *data);
void		env_remove_node(t_env **list, char *key);

// ------ BUILT IN HELPERS ----- //
int			count_ac(char **args);
int			count_strings(char **array);

// ----- ENV SET UP ----- //
int			set_environment(char **envp, t_data *data);
void		print_env_list(t_env *env_head);
int			set_environment(char **bash_env, t_data *data);
t_env		*env_node_new(char *key, char *value);
char		*get_key(char *str);
char		*get_value(char *str);
void		env_delone(t_env *node);
void		env_list_clear(t_env **env_list);
t_env		*env_last(t_env *env_head);
void		env_add_back(t_env **env_head, t_env *new_node);
t_env		*env_to_list(char **bash_env);
char		**env_to_array(t_env *env_head);
char		*ft_getenv(t_env *env, char *key);
int			env_lst_size(t_env *env);

// ----- MINISHELL SHUT DOWN ----- //
void		shut_down_minishell(t_data *data);
void		close_fds(t_command *cmd);
void		clean_up_exit(t_data *data, int exit_status, char *msg);

// ----- MISC UTILS ----- //
char		**duplicate_str_array(char **start_env);
void		free_str_array(char **array, int size);
void		new_log_timestamp(int fd, char *message);
int			count_strings(char **array);

#endif
