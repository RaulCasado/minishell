# Nombre del ejecutable
NAME = minishell

# Compilador y flags
CC = cc
CFLAGS = -Wall -Wextra -Werror
CFLAGS2 = 

# Directorios
SRC_DIR = src
INCLUDE_DIR = includes
LIBFT_DIR = includes/libft

# Archivos fuente
SRCS =	$(SRC_DIR)/main.c $(SRC_DIR)/signals.c $(SRC_DIR)/memory.c \
		$(SRC_DIR)/minishell_builder.c \
		$(SRC_DIR)/env.c \
		$(SRC_DIR)/str_utils.c \
		$(SRC_DIR)/token_builder.c $(SRC_DIR)/token_expander.c $(SRC_DIR)/token_assignator.c \
		$(SRC_DIR)/token_parser.c $(SRC_DIR)/token_utils.c \
		$(SRC_DIR)/command_creator.c $(SRC_DIR)/command_executer.c  $(SRC_DIR)/command_process.c \
		$(SRC_DIR)/builtin_echo.c $(SRC_DIR)/builtin_cd.c $(SRC_DIR)/builtin_pwd.c $(SRC_DIR)/builtin_export.c \
		$(SRC_DIR)/builtin_unset.c $(SRC_DIR)/builtin_env.c $(SRC_DIR)/builtin_exit.c $(SRC_DIR)/redirections.c 
OBJS = $(SRCS:.c=.o)

# Librería Libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_MAKEFILE = $(LIBFT_DIR)/Makefile

# Regla principal
all: $(LIBFT) $(NAME)

# Compilar libft primero si no existe libft.a
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

# Compilar Minishell
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS2) -I$(INCLUDE_DIR) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -lncurses -o $(NAME)

# Regla para compilar archivos .o
%.o: %.c
	$(CC) $(CFLAGS2) -I$(INCLUDE_DIR) -c $< -o $@

# Limpiar archivos objeto
clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

# Limpiar todo (binarios y objetos)
fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

# Recompilar desde cero
re: fclean all

# No usar archivos intermedios como targets
.PHONY: all clean fclean re
