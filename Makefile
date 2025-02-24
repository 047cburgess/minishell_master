NAME = minishell
CFLAGS = -Wall -Werror -Wextra -MMD -MP -g 

SRCS_DIR = srcs
INC_DIR = includes
OBJS_DIR = objs
LIBFT_DIR = libft
LIBFT = libft/libft.a

SRCS = $(SRCS_DIR)/main.c \
       $(SRCS_DIR)/signals.c \
       $(SRCS_DIR)/echo.c \
	   $(SRCS_DIR)/pwd.c \
	   $(SRCS_DIR)/cd.c \
	   $(SRCS_DIR)/parsing.c \
	   $(SRCS_DIR)/handle_quotes.c \
	   $(SRCS_DIR)/copy_env.c \
	   $(SRCS_DIR)/set_up.c \
	   $(SRCS_DIR)/clean_up.c \
	   $(SRCS_DIR)/extract_expansion.c \
	   $(SRCS_DIR)/mapping.c \
	   $(SRCS_DIR)/token_utils.c \
	   $(SRCS_DIR)/get_tokens.c \
	   $(SRCS_DIR)/executor.c \
	   $(SRCS_DIR)/executor2.c \
	   $(SRCS_DIR)/parse_tokens.c 

	   
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	cc $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -lncurses  -o $(NAME) 

$(LIBFT): 
	@make -C $(LIBFT_DIR)

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	cc $(CFLAGS) -I$(LIBFT_DIR) -I$(INC_DIR) -c $< -o $@

clean:
	@rm -rf $(OBJS_DIR)
	@make clean -C $(LIBFT_DIR)

fclean: clean
	@rm -rf $(NAME)
	@make fclean -C $(LIBFT_DIR)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
