#ifndef MAIN_H
# define MAIN_H

# include "minirt.h"

# define WIDTH 800
# define HEIGHT 600
# define WINDOW_NAME_RT "miniRT"

void	draw_new_image(t_vars *vars, t_scene *scene);
void	create_image(t_vars *vars);
void	cleanup_image(t_vars *vars);
void	main_draw(t_vars *vars, t_scene *scene);
void	put_pixel(t_vars *vars, int x, int y, int color);
void	cleanup_all(t_vars *vars);
void	error_exit(char *message);
void	print_scene_info(t_scene *scene);

#endif