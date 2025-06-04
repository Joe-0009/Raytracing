#ifndef MATH_UTILS_H
# define MATH_UTILS_H

# include <math.h>

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
} t_vec3;

typedef struct s_quadratic
{
	double	a;
	double	b;
	double	c;
} t_quadratic;

typedef t_vec3 t_point3;
typedef t_vec3 t_color3;

t_vec3	vec3_create(double x, double y, double z);
t_vec3	vec3_add(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_sub(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_mult(t_vec3 v, double t);
t_vec3	vec3_div(t_vec3 v, double t);
t_vec3	vec3_cross(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_normalize(t_vec3 v);
double	vec3_dot(t_vec3 v1, t_vec3 v2);
double	vec3_length(t_vec3 v);
double	vec3_length_squared(t_vec3 v);
t_vec3	reflect(t_vec3 v, t_vec3 n);
t_vec3	vec3_rotate_around_axis(t_vec3 v, t_vec3 axis, double angle);
double	solve_quadratic(double a, double b, double c, double min_t);

#endif
