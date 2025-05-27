
#include "../../includes/math_utils.h"
#include <math.h>

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

double	vec3_dot(t_vec3 v1, t_vec3 v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

t_vec3	vec3_cross(t_vec3 v1, t_vec3 v2)
{
	return (vec3_create(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x));
}

double	vec3_length_squared(t_vec3 v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

double	vec3_length(t_vec3 v)
{
	return (sqrt(vec3_length_squared(v)));
}

static t_vec3	handle_zero_vector(void)
{
	return (vec3_create(0, 0, 1));
}

t_vec3	vec3_normalize(t_vec3 v)
{
	double	length;

	length = vec3_length(v);
	if (length < 0.0000001)
		return (handle_zero_vector());
	return (vec3_div(v, length));
}

t_vec3	reflect(t_vec3 v, t_vec3 n)
{
	return (vec3_sub(v, vec3_mult(n, 2 * vec3_dot(v, n))));
}
