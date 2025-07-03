# Makefile for miniRT

NAME = miniRT
BONUS_NAME = minirt_bonus

# Compiler and flags
CC = gcc
# Performance-optimized flags for raytracing rendering (Clang-compatible)
CFLAGS = -Wall -Wextra -Werror -I./mandatory/includes -I../minilibx-linux \
         -O3 -ffast-math -funroll-loops -ftree-vectorize \
         -fomit-frame-pointer -march=native -mtune=native \
         -flto -finline-functions \
         -falign-functions=32 -falign-loops=32 \
         -fno-stack-protector -fno-unwind-tables \
         -fno-asynchronous-unwind-tables \
         -DNDEBUG
LDFLAGS = -L../minilibx-linux -lmlx -lXext -lX11 -lm -lz
INCLUDES = -I./mandatory/includes
# Libraries
LIBFT_DIR = mandatory/libft
LIBFT = $(LIBFT_DIR)/libft.a

# Source files
SRC_DIR = mandatory/src
BONUS_DIR = bonus/src
OBJ_DIR = obj
BONUS_OBJ_DIR = obj_bonus

# Bonus-specific variables
BONUS_LIBFT_DIR = bonus/libft
BONUS_LIBFT = $(BONUS_LIBFT_DIR)/libft.a
BONUS_INCLUDES = -I./bonus/includes
BONUS_CFLAGS = -Wall -Wextra -Werror -I./bonus/includes -I../minilibx-linux \
         -O3 -ffast-math -funroll-loops -ftree-vectorize \
         -fomit-frame-pointer -march=native -mtune=native \
         -flto -finline-functions \
         -falign-functions=32 -falign-loops=32 \
         -fno-stack-protector -fno-unwind-tables \
         -fno-asynchronous-unwind-tables \
         -DNDEBUG

# Find all .c files recursively
SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Bonus files
BONUS_SRCS = $(shell find $(BONUS_DIR) -name '*_bonus.c' 2>/dev/null || echo "")
BONUS_OBJS = $(BONUS_SRCS:$(BONUS_DIR)/%.c=$(BONUS_OBJ_DIR)/%.o)

# Rules
all: $(LIBFT) $(NAME)

# Bonus target
bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS) $(BONUS_LIBFT)
	$(CC) $(CFLAGS) $(BONUS_OBJS) -o $(BONUS_NAME) -L$(BONUS_LIBFT_DIR) -lft $(LDFLAGS)

$(BONUS_OBJ_DIR)/%.o: $(BONUS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(BONUS_CFLAGS) $(BONUS_INCLUDES) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(BONUS_LIBFT):
	@make -C $(BONUS_LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(BONUS_OBJ_DIR)
	@make -C $(LIBFT_DIR) clean
	@make -C $(BONUS_LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME) $(BONUS_NAME)
	@make -C $(LIBFT_DIR) fclean
	@make -C $(BONUS_LIBFT_DIR) fclean

re: fclean all

# Bonus re-compilation
re_bonus: fclean bonus

.PHONY: all bonus clean fclean re re_bonus
