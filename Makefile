NAME = minishell
CFLAGS = -Wall -Werror -Wextra -MMD -MP -g -fsanitize=address 

##----- DIRECTORIES -----##
PARSE_DIR = Parsing
ENV_DIR = Environment
CLEAN_DIR = Clean
EXEC_DIR = Exec
EXPAND_DIR = Expansions
SIGNALS_DIR = Signals
BUILTIN_DIR = Builtins
INC_DIR = Includes
OBJS_DIR = Objs
LIBFT_DIR = libft
DPRINTF_DIR = printf_fd
LIBFT = libft/libft.a
DPRINTF = printf_fd/libftdprintf.a

##----- FILES -----##

SRCS = main.c \
	   $(ENV_DIR)/copy_env.c \
	   $(ENV_DIR)/set_up.c \
	   $(ENV_DIR)/env_utils.c \
	   $(SIGNALS_DIR)/signals.c \
	   $(PARSE_DIR)/parsing.c \
	   $(PARSE_DIR)/get_tokens.c \
	   $(PARSE_DIR)/handle_tokens.c \
	   $(PARSE_DIR)/handle_quotes.c \
	   $(PARSE_DIR)/token_utils.c \
	   $(PARSE_DIR)/parse_tokens.c \
	   $(PARSE_DIR)/heredocs.c \
	   $(EXPAND_DIR)/extract_expansion.c \
	    $(EXPAND_DIR)/expansions_cutting.c \
	   $(BUILTIN_DIR)/echo.c \
	   $(BUILTIN_DIR)/pwd.c \
	   $(BUILTIN_DIR)/cd.c \
		$(BUILTIN_DIR)/export.c \
	   $(BUILTIN_DIR)/unset.c \
	   $(BUILTIN_DIR)/env.c \
	   $(EXEC_DIR)/prep_command_tables.c \
	   $(EXEC_DIR)/command_list.c \
	   $(EXEC_DIR)/command_table_utils.c \
	   $(EXEC_DIR)/executor.c \
	   $(EXEC_DIR)/exec_utils.c \
	   $(EXEC_DIR)/return_status.c \
	   $(EXEC_DIR)/redirections.c \
	   $(EXEC_DIR)/redirections_utils.c \
	   $(EXEC_DIR)/child_errors.c \
	   $(EXEC_DIR)/fd_management.c \
	   $(EXEC_DIR)/pipeline2.c \
	   $(CLEAN_DIR)/clean_up.c \

OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))
DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(LIBFT) $(DPRINTF) $(OBJS)
	cc $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -L $(DPRINTF_DIR) -lft -lftdprintf -lreadline -lncurses  -o $(NAME) 

$(LIBFT): 
	@make -C $(LIBFT_DIR)

$(DPRINTF):
	@make -C $(DPRINTF_DIR)

$(OBJS_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	cc $(CFLAGS) -I$(LIBFT_DIR) -I$(DPRINTF_DIR) -I$(INC_DIR) -c $< -o $@

clean:
	@rm -rf $(OBJS_DIR)
	@make clean -C $(LIBFT_DIR)
	@make clean -C $(DPRINTF_DIR)

fclean: clean
	@rm -rf $(NAME)
	@make fclean -C $(LIBFT_DIR)
	@make fclean -C $(DPRINTF_DIR)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
