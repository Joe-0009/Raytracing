
#ifndef MAIN_H
# define MAIN_H


# include "minirt.h"

/* Window dimensions */
# define WIDTH 1600
# define HEIGHT 1600
# define window_name_rt "miniRT"

/* Image structure */
typedef struct s_image
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_image;

/* Main program variables structure */
typedef struct s_vars
{
	void		*mlx;
	void		*win;
	t_image		*img;
}	t_vars;

/* Function prototypes for image handling */
void	draw_new_image(t_vars *vars);
void	create_image(t_vars *vars);
void	cleanup_image(t_vars *vars);
void	main_draw(t_vars *vars);
void	put_pixel(t_vars *vars, int x, int y, int color);

/* Cleanup functions */
void	cleanup_all(t_vars *vars);

/* Error handling */
void	error_exit(char *message);

/* Scene utilities */
void	print_scene_info(t_scene *scene);

#endif