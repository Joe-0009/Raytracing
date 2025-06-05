# Makefile for miniRT

NAME = miniRT

# Compiler and flags
CC = gcc
# Performance-optimized flags for raytracing rendering (Clang-compatible)
CFLAGS = -Wall -Wextra -Werror -I./includes -I../minilibx-linux \
         -O3 -ffast-math -funroll-loops -ftree-vectorize \
         -fomit-frame-pointer -march=native -mtune=native \
         -flto -finline-functions \
         -falign-functions=32 -falign-loops=32 \
         -fno-stack-protector -fno-unwind-tables \
         -fno-asynchronous-unwind-tables \
         -DNDEBUG
LDFLAGS = -L../minilibx-linux -lmlx -lXext -lX11 -lm
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

# Debug build with debugging symbols and no optimization
debug: CFLAGS = -Wall -Wextra -Werror -I./includes -I../minilibx_macos -g -O0 -DDEBUG
debug: $(LIBFT) $(NAME)

# Profile build with profiling enabled
profile: CFLAGS += -pg -fprofile-arcs
profile: LDFLAGS += -pg
profile: $(LIBFT) $(NAME)

# Ultra-optimized build for maximum performance (experimental)
ultra: CFLAGS = -Wall -Wextra -Werror -I./includes -I../minilibx_macos \
         -Ofast -ffast-math -funroll-loops -ftree-vectorize \
         -floop-interchange -floop-unroll-and-jam \
         -fprefetch-loop-arrays -falign-functions=64 \
         -falign-loops=64 -fomit-frame-pointer \
         -march=native -mtune=native -mcpu=native \
         -flto -fwhole-program -fuse-linker-plugin \
         -finline-functions -finline-limit=5000 \
         -fgcse-after-reload -fipa-cp-clone \
         -fmodulo-sched -fmodulo-sched-allow-regmoves \
         -ftree-loop-distribute-patterns -ftree-loop-im \
         -ftree-loop-ivcanon -floop-nest-optimize \
         -fgraphite-identity -floop-parallelize-all \
         -fpredictive-commoning -fsplit-wide-types \
         -ftree-partial-pre -fvect-cost-model=unlimited \
         -falign-jumps=64 -falign-labels=64 \
         -fno-stack-protector -fno-unwind-tables \
         -fno-asynchronous-unwind-tables -fno-plt \
         -fdevirtualize-at-ltrans -fipa-pta \
         -ftracer -frename-registers \
         -DNDEBUG -DULTRA_FAST
ultra: LDFLAGS += -flto -fuse-linker-plugin -Wl,-O3,-sort-common,--as-needed
ultra: $(LIBFT) $(NAME)

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

.PHONY: all clean fclean re debug profile ultra test test-re
