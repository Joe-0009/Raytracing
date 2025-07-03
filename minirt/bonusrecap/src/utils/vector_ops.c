#include "../includes/scene_math.h"
#include <math.h>

// Create a new vector
t_vec3	vec3_create(double x, double y, double z)
{
	t_vec3	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

t_vec3	vec3_add(t_vec3 v1, t_vec3 v2)
{
	return (vec3_create(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z));
}

t_vec3	vec3_sub(t_vec3 v1, t_vec3 v2)
{
	return (vec3_create(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z));
}

t_vec3	vec3_mult(t_vec3 v, double t)
{
	return (vec3_create(v.x * t, v.y * t, v.z * t));
}

t_vec3	vec3_div(t_vec3 v, double t)
{
	return (vec3_create(v.x / t, v.y / t, v.z / t));
}

t_vec3	vec3_cross(t_vec3 v1, t_vec3 v2)
{
	return (vec3_create(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x));
}

double	vec3_dot(t_vec3 v1, t_vec3 v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

double	vec3_length_squared(t_vec3 v)
{
	return (vec3_dot(v, v));
}

double	vec3_length(t_vec3 v)
{
	return (sqrt(vec3_length_squared(v)));
}

t_vec3	vec3_normalize(t_vec3 v)
{
	double	len;

	len = vec3_length(v);
	if (len == 0)
		return (vec3_create(0, 0, 0));
	return (vec3_div(v, len));
}

t_vec3	reflect(t_vec3 v, t_vec3 n)
{
	return (vec3_sub(v, vec3_mult(n, 2 * vec3_dot(v, n))));
}

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
