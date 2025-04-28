# Executable name
NAME = minishell

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror
CFLAGS2 = 

# Directories
SRC_DIR = src
INCLUDE_DIR = includes
LIBFT_DIR = includes/libft

# Source files
SRCS =	$(SRC_DIR)/main.c $(SRC_DIR)/memory.c $(SRC_DIR)/memory_extras.c \
		$(SRC_DIR)/minishell_builder.c \
		$(SRC_DIR)/env.c $(SRC_DIR)/signals.c $(SRC_DIR)/signals_global.c \
		$(SRC_DIR)/str_utils.c \
		$(SRC_DIR)/token_builder.c $(SRC_DIR)/token_builder_utils.c $(SRC_DIR)/token_expander.c \
		$(SRC_DIR)/token_expander_utils.c $(SRC_DIR)/token_assignator.c $(SRC_DIR)/token_parser.c \
		$(SRC_DIR)/token_utils.c $(SRC_DIR)/token_unquoter.c \
		$(SRC_DIR)/command_creator.c $(SRC_DIR)/command_executer.c $(SRC_DIR)/command_executer_helper.c $(SRC_DIR)/command_executer_utils.c $(SRC_DIR)/command_process.c \
		$(SRC_DIR)/builtin_echo.c $(SRC_DIR)/builtin_cd.c $(SRC_DIR)/builtin_pwd.c $(SRC_DIR)/builtin_export.c \
		$(SRC_DIR)/builtin_unset.c $(SRC_DIR)/builtin_env.c $(SRC_DIR)/builtin_exit.c $(SRC_DIR)/redirections.c \
		$(SRC_DIR)/command_creator_utils.c $(SRC_DIR)/command_decider.c $(SRC_DIR)/command_process_utils.c $(SRC_DIR)/command_creator_helper.c \
		$(SRC_DIR)/main_helper.c $(SRC_DIR)/command_executer_redirections.c $(SRC_DIR)/heredoc.c

OBJS = $(SRCS:.c=.o)

# Libft library
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_MAKEFILE = $(LIBFT_DIR)/Makefile

# Main rule
all: $(LIBFT) $(NAME)

# Compile libft first if libft.a doesn't exist
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

# Compile Minishell
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -lncurses -o $(NAME)

# Rule to compile .o files
%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Clean object files
clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

# Clean everything (binaries and objects)
fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

# Recompile from scratch
re: fclean all

# Don't use intermediate files as targets
.PHONY: all clean fclean re
