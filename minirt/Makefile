# Makefile for miniRT

NAME = miniRT

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lmlx -lm -lXext -lX11 -lz
INCLUDES = -I./includes

# Libraries
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Source files
SRC_DIR = src
OBJ_DIR = obj

# Find all .c files recursively
SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Rules
all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

# Run tests
test: all
	@echo "Running tests..."
	@cd tests && make

# Run tests with clean logs
test-re: all
	@echo "Cleaning test logs and running tests..."
	@cd tests && make re

.PHONY: all clean fclean re test test-re
