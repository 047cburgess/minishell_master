NAME = minishell
CFLAGS = -Wall -Werror -Wextra -MMD -MP -g 

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
LIBFT = libft/libft.a

##----- FILES -----##

SRCS = main.c \
	   $(ENV_DIR)/copy_env.c \
	   $(ENV_DIR)/set_up.c \
	   $(SIGNALS_DIR)/signals.c \
	   $(PARSE_DIR)/parsing.c \
	   $(PARSE_DIR)/get_tokens.c \
	   $(PARSE_DIR)/handle_quotes.c \
	   $(PARSE_DIR)/token_utils.c \
	   $(PARSE_DIR)/parse_tokens.c \
	   $(EXPAND_DIR)/extract_expansion.c \
	   $(EXPAND_DIR)/mapping.c \
	   $(BUILTIN_DIR)/echo.c \
	   $(BUILTIN_DIR)/pwd.c \
	   $(BUILTIN_DIR)/cd.c \
	   $(EXEC_DIR)/executor.c \
	   $(EXEC_DIR)/executor2.c \
	   $(EXEC_DIR)/command_list_utils.c \
	   $(EXEC_DIR)/command_table_helpers.c \
	   $(EXEC_DIR)/return_status.c \
	   $(CLEAN_DIR)/clean_up.c \

	   
OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))
DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	cc $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -lncurses  -o $(NAME) 

$(LIBFT): 
	@make -C $(LIBFT_DIR)


$(OBJS_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	cc $(CFLAGS) -I$(LIBFT_DIR) -I $(INC_DIR) -c $< -o $@

clean:
	@rm -rf $(OBJS_DIR)
	@make clean -C $(LIBFT_DIR)

fclean: clean
	@rm -rf $(NAME)
	@make fclean -C $(LIBFT_DIR)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
