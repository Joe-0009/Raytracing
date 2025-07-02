#include "../includes/math_utils.h"
#include <math.h>

/*
** Reflect vector across normal
*/
t_vec3	reflect(t_vec3 v, t_vec3 n)
{
	return (vec3_sub(v, vec3_mult(n, 2 * vec3_dot(v, n))));
}

/*
** Rotate vector around arbitrary axis
*/
t_vec3	vec3_rotate_around_axis(t_vec3 v, t_vec3 axis, double angle)
{
	t_vec3	u;
	double	cos_a;
	double	sin_a;

	u = vec3_normalize(axis);
	cos_a = cos(angle);
	sin_a = sin(angle);
	return (vec3_add(vec3_add(vec3_mult(v, cos_a), vec3_mult(vec3_cross(u, v),
					sin_a)), vec3_mult(u, vec3_dot(u, v) * (1 - cos_a))));
}
