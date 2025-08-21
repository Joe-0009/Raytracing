/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_ops2_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:20:36 by isallali          #+#    #+#             */
/*   Updated: 2025/08/21 21:20:37 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/math_utils_bonus.h"
#include <math.h>

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

// Rodrigues' Rotation Formula

t_vec3	vec3_rotate(t_vec3 v, t_vec3 rotation)
{
	if (rotation.x != 0)
		v = vec3_rotate_around_axis(v, vec3_create(1, 0, 0), rotation.x);
	if (rotation.y != 0)
		v = vec3_rotate_around_axis(v, vec3_create(0, 1, 0), rotation.y);
	if (rotation.z != 0)
		v = vec3_rotate_around_axis(v, vec3_create(0, 0, 1), rotation.z);
	return (v);
}
