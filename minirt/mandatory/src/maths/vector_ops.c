#include "../includes/math_utils.h"
#include <math.h>

/*
** Create vector with given coordinates
*/
t_vec3	vec3_create(double x, double y, double z)
{
	t_vec3	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

/*
** Add two vectors
*/
t_vec3	vec3_add(t_vec3 v1, t_vec3 v2)
{
	return (vec3_create(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z));
}

/*
** Subtract two vectors
*/
t_vec3	vec3_sub(t_vec3 v1, t_vec3 v2)
{
	return (vec3_create(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z));
}

/*
** Multiply vector by scalar
*/
t_vec3	vec3_mult(t_vec3 v, double t)
{
	return (vec3_create(v.x * t, v.y * t, v.z * t));
}

/*
** Divide vector by scalar
*/
t_vec3	vec3_div(t_vec3 v, double t)
{
	return (vec3_create(v.x / t, v.y / t, v.z / t));
}
