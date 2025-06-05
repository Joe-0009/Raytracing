#include "../includes/minirt_app.h"
#include <math.h>

/*
** Convert color from [0-1] to int format for display
*/
int	color_to_int(t_color3 color)
{
	return ((int)(color.x * 255.0) << 16)
		| ((int)(color.y * 255.0) << 8)
		| (int)(color.z * 255.0);
}

/*
** Calculate sky gradient color based on ray direction
*/
int	get_sky_color(t_ray ray)
{
	double	grad;

	grad = 0.5 * (ray.direction.y + 1.0);
	return ((int)((1.0 - grad) * 255 + grad * 135) << 16)
		| ((int)((1.0 - grad) * 255 + grad * 206) << 8)
		| (int)((1.0 - grad) * 255 + grad * 235);
}

/*
** Clamp color values to [0, 1]
*/
t_color3	clamp_color(t_color3 color)
{
	color.x = fmin(1.0, fmax(0.0, color.x));
	color.y = fmin(1.0, fmax(0.0, color.y));
	color.z = fmin(1.0, fmax(0.0, color.z));
	return (color);
}
