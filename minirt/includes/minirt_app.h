#ifndef MINIRT_APP_H
# define MINIRT_APP_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <float.h>
# include <limits.h>
# include <math.h>
# include <mlx.h>
# include <stdlib.h>
# include <unistd.h>

# define TRUE 1
# define FALSE 0

# define WIDTH 800
# define HEIGHT 600
# define WINDOW_NAME_RT "miniRT"

# include "intersections.h"
# include "parser.h"
# include "scene_math.h"

/* Error codes */
# define ERR_ARGS "Error: Invalid number of arguments\n"
# define ERR_FILE "Error: Could not open file\n"
# define ERR_FORMAT "Error: Invalid file format\n"
# define ERR_SCENE "Error: Invalid scene configuration\n"
# define ERR_MEMORY "Error: Memory allocation failed\n"
# define ERR_FILE_FORMAT "Error: File must have .rt extension\n"

/* Image structure */
typedef struct s_image
{
	void				*img;
	char				*addr;
	int					bits_per_pixel;
	int					line_length;
	int					endian;
}						t_image;

/* Main program variables structure */
typedef struct s_vars
{
	void				*mlx;
	void				*win;
	t_image				*img;
}						t_vars;

typedef struct s_hit	t_hit;

/* Function prototypes */
void					draw_new_image(t_vars *vars, t_scene *scene);
void					create_image(t_vars *vars);
void					cleanup_image(t_vars *vars);
void					main_draw(t_vars *vars, t_scene *scene);
void					put_pixel(t_vars *vars, int x, int y, int color);
void					cleanup_all(t_vars *vars);
void					error_exit(char *message);
void					print_scene_info(t_scene *scene);

/* Error utility functions */
void					ft_print_error(const char *message);
void					ft_print_error_detail(const char *message,
							const char *detail);
void					ft_print_number(double num);
void					ft_print_vector(t_vec3 v);

/* Color utilities */
int						color_to_int(t_color3 color);
int						get_sky_color(t_ray ray);
t_color3				clamp_color(t_color3 color);

/* Lighting utilities */
t_color3				calculate_ambient(const t_scene *scene,
							const t_hit *hit);
t_color3				calculate_diffuse(const t_scene *scene,
							const t_hit *hit);
int						is_in_shadow(const t_scene *scene, const t_vec3 point,
							const t_vec3 light_pos);
t_color3				calculate_lighting(const t_scene *scene,
							const t_hit *hit);

#endif
