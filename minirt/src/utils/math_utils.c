#include "../includes/math_utils.h"
#include <math.h>

/**
 * Helper function to solve quadratic equation ax^2 + bx + c = 0
 * Returns the smallest positive root > min_t, or -1 if none
 */
double	solve_quadratic(double a, double b, double c, double min_t)
{
	double	discriminant;
	double	sqrt_d;
	double	t0;
	double	t1;

	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1.0);
	sqrt_d = sqrt(discriminant);
	t0 = (-b - sqrt_d) / (2.0 * a);
	t1 = (-b + sqrt_d) / (2.0 * a);
	if (t0 > min_t && (t0 < t1 || t1 <= min_t))
		return (t0);
	if (t1 > min_t)
		return (t1);
	return (-1.0);
}

/*
** Rotate a vector around an axis by a given angle (in radians)
** Uses Rodrigues' rotation formula
*/
t_vec3	vec3_rotate_around_axis(t_vec3 v, t_vec3 axis, double angle)
{
	t_vec3	result;
	double	cos_angle;
	double	sin_angle;

	cos_angle = cos(angle);
	sin_angle = sin(angle);

	/* Rodrigues' rotation formula: v_rot = v*cos(θ) + (k×v)*sin(θ) + k(k·v)(1-cos(θ)) */
	result = vec3_mult(v, cos_angle);  // v*cos(θ)
	result = vec3_add(result, vec3_mult(vec3_cross(axis, v), sin_angle));  // + (k×v)*sin(θ)
	result = vec3_add(result, vec3_mult(axis, vec3_dot(axis, v) * (1.0 - cos_angle)));  // + k(k·v)(1-cos(θ))

	return (result);
}
