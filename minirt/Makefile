NAME = miniRT
BONUS_NAME = minirt_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror

# Directories
LIBFT_DIR = ../libft
LIBFT = $(LIBFT_DIR)/libft.a
MLX_DIR = ..

# Libraries
MLX_LDFLAGS = -L$(MLX_DIR) -lmlx_Linux -lX11 -lXext -lm
BONUS_LDFLAGS = -L$(MLX_DIR) -lmlx_Linux -lX11 -lXext -lm -pthread

# Includes
INCLUDES = -I./mandatory/includes -I$(LIBFT_DIR)
BONUS_INCLUDES = -I./bonus/includes -I$(LIBFT_DIR)

# Source files
SRC_DIR = mandatory/src
BONUS_DIR = bonus/src

# Source files
SRCS = events/camera_light_handlers.c \
       events/events.c \
       events/object_handlers.c \
       events/transform_operations.c \
       events/window_handlers.c \
       main.c \
       maths/math_utils.c \
       maths/vector_ops1.c \
       maths/vector_ops2.c \
       maths/vector_ops.c \
       parser/add_elements_validate_scene.c \
       parser/parse_double_color_vector.c \
       parser/parse_elements.c \
       parser/parse_file.c \
       parser/parser_utils.c \
       parser/validate_elements.c \
       render/color_utils.c \
       render/draw.c \
       render/intersect_cylinder.c \
       render/intersections.c \
       render/intersect_plane.c \
       render/intersect_sphere.c \
       render/lighting.c \
       render/raytrace.c

OBJS = $(SRCS:%.c=$(SRC_DIR)/%.o)

BONUS_SRCS = events/camera_light_handlers_bonus.c \
             events/events_bonus.c \
             events/object_handlers_bonus.c \
             events/transform_operations_bonus.c \
             events/window_handlers_bonus.c \
             main_bonus.c \
             maths/math_utils_bonus.c \
             maths/vector_ops1_bonus.c \
             maths/vector_ops2_bonus.c \
             maths/vector_ops_bonus.c \
             parser/add_elements_validate_scene_bonus.c \
             parser/parse_double_color_vector_bonus.c \
             parser/parse_elements2_bonus.c \
             parser/parse_elements_bonus.c \
             parser/parse_file_bonus.c \
             parser/parser_utils_bonus.c \
             parser/parse_texture_bump_map_bonus.c \
             parser/validate_elements_bonus.c \
             render/color_utils_bonus.c \
             render/draw_bonus.c \
             render/intersect_cone_bonus.c \
             render/intersect_cylinder_bonus.c \
             render/intersections_bonus.c \
             render/intersect_plane_bonus.c \
             render/intersect_sphere_bonus.c \
             render/lighting_bonus.c \
             render/raytrace_bonus.c \
             render/texture_load_bonus.c

BONUS_OBJS = $(BONUS_SRCS:%.c=$(BONUS_DIR)/%.o)

all: $(NAME)

bonus: $(BONUS_NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(MLX_LDFLAGS) $(LIBFT)

$(BONUS_NAME): $(LIBFT) $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(BONUS_OBJS) -o $(BONUS_NAME) $(BONUS_LDFLAGS) $(LIBFT)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BONUS_DIR)/%.o: $(BONUS_DIR)/%.c
	$(CC) $(CFLAGS) $(BONUS_INCLUDES) -c $< -o $@

clean:
	@rm -f $(OBJS) $(BONUS_OBJS)
	@make clean -C $(LIBFT_DIR)

fclean: clean
	@rm -f $(NAME) $(BONUS_NAME)
	@make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all bonus clean fclean re
.SECONDARY: $(OBJS) $(BONUS_OBJS)
