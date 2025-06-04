#ifndef COLOR_UTILS_H
# define COLOR_UTILS_H

# include "math_utils.h"
# include "raytrace.h"

int		color_to_int(t_color3 color);
int		get_sky_color(t_ray ray);
t_color3	clamp_color(t_color3 color);

#endif
