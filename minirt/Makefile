# Makefile for miniRT

NAME = miniRT
BONUS_NAME = minirt_bonus

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -O3 -ffast-math -funroll-loops -ftree-vectorize \
		 -fomit-frame-pointer -march=native -mtune=native -flto -finline-functions \
		 -falign-functions=32 -falign-loops=32 -fno-stack-protector \
		 -fno-unwind-tables -fno-asynchronous-unwind-tables -DNDEBUG

# Prebuilt libraries
PREBUILT_DIR = ../prebuilt_libs
LIBFT = $(PREBUILT_DIR)/libft.a

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
MLX_LDFLAGS = -L$(PREBUILT_DIR) -lmlx -framework OpenGL -framework AppKit
BONUS_LDFLAGS = -L$(PREBUILT_DIR) -lmlx -framework OpenGL -framework AppKit
else
MLX_LDFLAGS = -L$(PREBUILT_DIR) -lmlx_linux -lX11 -lXext -lm
BONUS_LDFLAGS = -L$(PREBUILT_DIR) -lmlx_linux -lX11 -lXext -lm -pthread
endif

# Includes
INCLUDES = -I./mandatory/includes -I../prebuilt_libs/include
BONUS_INCLUDES = -I./bonus/includes -I../prebuilt_libs/include

# Source files
SRC_DIR = mandatory/src
BONUS_DIR = bonus/src
OBJ_DIR = obj
BONUS_OBJ_DIR = obj_bonus

# Find all .c files
SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

BONUS_SRCS = $(shell find $(BONUS_DIR) -name '*_bonus.c' 2>/dev/null || echo "")
BONUS_OBJS = $(BONUS_SRCS:$(BONUS_DIR)/%.c=$(BONUS_OBJ_DIR)/%.o)

# Rules

all: $(NAME)

bonus: $(BONUS_NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(MLX_LDFLAGS) $(LIBFT)

$(BONUS_NAME): $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(BONUS_OBJS) -o $(BONUS_NAME) $(BONUS_LDFLAGS) $(LIBFT)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BONUS_OBJ_DIR)/%.o: $(BONUS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(BONUS_INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(BONUS_OBJ_DIR)

fclean: clean
	@rm -f $(NAME) $(BONUS_NAME)

re: fclean all

.PHONY: all bonus clean fclean re
