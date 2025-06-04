#ifndef DRAW_H
# define DRAW_H

# include "main.h"
# include "scene.h"
# include "raytrace.h"

void	create_image(t_vars *vars);
void	put_pixel(t_vars *vars, int x, int y, int color);
void	main_draw(t_vars *vars, t_scene *scene);

#endif
