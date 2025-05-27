#include "../includes/color_utils.h"
#include "../includes/raytrace.h"
#include <math.h>

/*
 * Convert color from [0-1] to int format for display
 */
int	color_to_int(t_color3 color)
{
	int	r;
	int	g;
	int	b;

	r = (int)(color.x * 255.0);
	g = (int)(color.y * 255.0);
	b = (int)(color.z * 255.0);
	return ((r << 16) | (g << 8) | b);
}

/*
 * Calculate sky gradient color based on ray direction
 */
int	get_sky_color(t_ray ray)
{
	double	grad;
	int		r;
	int		g;
	int		b;

	grad = 0.5 * (ray.direction.y + 1.0);
	r = (int)((1.0 - grad) * 255 + grad * 135);
	g = (int)((1.0 - grad) * 255 + grad * 206);
	b = (int)((1.0 - grad) * 255 + grad * 235);
	return ((r << 16) | (g << 8) | b);
}

/*
 * Simple color utility function 
 */
t_color3	clamp_color(t_color3 color)
{
	if (color.x < 0.0)
		color.x = 0.0;
	else if (color.x > 1.0)
		color.x = 1.0;
	if (color.y < 0.0)
		color.y = 0.0;
	else if (color.y > 1.0)
		color.y = 1.0;
	if (color.z < 0.0)
		color.z = 0.0;
	else if (color.z > 1.0)
		color.z = 1.0;
	return (color);
}
