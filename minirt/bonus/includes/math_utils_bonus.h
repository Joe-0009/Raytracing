/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils_bonus.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:34:50 by isallali          #+#    #+#             */
/*   Updated: 2025/08/21 21:34:51 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_UTILS_BONUS_H
# define MATH_UTILS_BONUS_H

# include <math.h>

/* Math/vector types */
typedef struct s_vec3
{
	double		x;
	double		y;
	double		z;
}				t_vec3;

typedef struct s_quadratic
{
	double		a;
	double		b;
	double		c;
}				t_quadratic;

typedef t_vec3	t_point3;
typedef t_vec3	t_color3;

/* Math/vector utilities */
t_vec3			vec3_create(double x, double y, double z);
t_vec3			vec3_add(t_vec3 v1, t_vec3 v2);
t_vec3			vec3_sub(t_vec3 v1, t_vec3 v2);
t_vec3			vec3_mult(t_vec3 v, double t);
t_vec3			vec3_div(t_vec3 v, double t);
t_vec3			vec3_cross(t_vec3 v1, t_vec3 v2);
t_vec3			vec3_normalize(t_vec3 v);
double			vec3_dot(t_vec3 v1, t_vec3 v2);
double			vec3_length(t_vec3 v);
double			vec3_length_squared(t_vec3 v);
t_vec3			reflect(t_vec3 v, t_vec3 n);
t_vec3			vec3_rotate_around_axis(t_vec3 v, t_vec3 axis, double angle);
t_vec3			vec3_rotate(t_vec3 v, t_vec3 rotation);
double			solve_quadratic(double a, double b, double c, double min_t);

#endif
