#include "../includes/math_utils_bonus.h"
#include <math.h>

/*
** Calculate cross product of two vectors
*/
t_vec3	vec3_cross(t_vec3 v1, t_vec3 v2)
{
	return (vec3_create(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x));
}

/*
** Calculate dot product of two vectors
*/
double	vec3_dot(t_vec3 v1, t_vec3 v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

/*
** Calculate squared length of vector
*/
double	vec3_length_squared(t_vec3 v)
{
	return (vec3_dot(v, v));
}

/*
** Calculate length of vector
*/
double	vec3_length(t_vec3 v)
{
	return (sqrt(vec3_length_squared(v)));
}

/*
** Normalize vector to unit length
*/
t_vec3	vec3_normalize(t_vec3 v)
{
	double	len;

	len = vec3_length(v);
	if (len == 0)
		return (vec3_create(0, 0, 0));
	return (vec3_div(v, len));
}
