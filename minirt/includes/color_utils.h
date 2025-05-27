#ifndef COLOR_UTILS_H
# define COLOR_UTILS_H

# include "math_utils.h"
# include "raytrace.h"

/* Forward declarations */
typedef struct s_hit t_hit;
typedef struct s_scene t_scene;

/* 
 * Color utility functions 
 */

/* Convert color from [0-1] range to int format for display */
int     color_to_int(t_color3 color);

/* Calculate sky gradient color based on ray direction */
int     get_sky_color(t_ray ray);

/* Simple color utilities */
t_color3 clamp_color(t_color3 color);

#endif
