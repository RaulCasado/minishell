# Nombre del ejecutable
NAME = minishell

# Compilador y flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Directorios
SRC_DIR = src
INCLUDE_DIR = includes
LIBFT_DIR = includes/libft

# Archivos fuente
SRCS = $(SRC_DIR)/main.c
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
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(OBJS) -L$(LIBFT_DIR) -lft -o $(NAME)

# Regla para compilar archivos .o
%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

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
