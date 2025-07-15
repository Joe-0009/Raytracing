#ifndef MATH_UTILS_BONUS_H
# define MATH_UTILS_BONUS_H

# include <math.h>

/* Math/vector types */
typedef struct s_vec3
{
	double			x;
	double			y;
	double			z;
}					t_vec3;

typedef struct s_quadratic
{
	double			a;
	double			b;
	double			c;
}					t_quadratic;

typedef t_vec3		t_point3;
typedef t_vec3		t_color3;

/* Matrix and transform types */
typedef struct s_matrix4
{
	double			m[4][4];
}					t_matrix4;

/* Math/vector utilities */
t_vec3				vec3_create(double x, double y, double z);
t_vec3				vec3_add(t_vec3 v1, t_vec3 v2);
t_vec3				vec3_sub(t_vec3 v1, t_vec3 v2);
t_vec3				vec3_mult(t_vec3 v, double t);
t_vec3				vec3_div(t_vec3 v, double t);
t_vec3				vec3_cross(t_vec3 v1, t_vec3 v2);
t_vec3				vec3_normalize(t_vec3 v);
double				vec3_dot(t_vec3 v1, t_vec3 v2);
double				vec3_length(t_vec3 v);
double				vec3_length_squared(t_vec3 v);
t_vec3				reflect(t_vec3 v, t_vec3 n);
t_vec3				vec3_rotate_around_axis(t_vec3 v, t_vec3 axis,
						double angle);
double				solve_quadratic(double a, double b, double c, double min_t);

/* Matrix operations */
t_matrix4			matrix4_identity(void);
t_matrix4			matrix4_multiply(t_matrix4 a, t_matrix4 b);
t_matrix4			matrix4_translation(t_vec3 translation);
t_matrix4			matrix4_rotation_x(double angle);
t_matrix4			matrix4_rotation_y(double angle);
t_matrix4			matrix4_rotation_z(double angle);
t_matrix4			matrix4_scale(t_vec3 scale);
t_vec3				matrix4_transform_point(t_matrix4 m, t_vec3 point);
t_vec3				matrix4_transform_direction(t_matrix4 m, t_vec3 direction);

#endif
