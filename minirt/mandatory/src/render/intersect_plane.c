/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_plane.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:30:26 by isallali          #+#    #+#             */
/*   Updated: 2025/08/21 21:30:27 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt_app.h"
#include "../includes/scene.h"
#include <math.h>

/*
** Compute the quadratic coefficients for a ray-cylinder intersection.
** Returns a t_quadratic struct with the coefficients a, b, c.
*/
t_quadratic	cylinder_quadratic_coeffs(const t_cylinder *cylinder, t_ray ray)
{
	t_vec3		oc;
	t_vec3		ray_axis_cross;
	t_vec3		oc_axis_cross;
	double		radius;
	t_quadratic	q;

	oc = vec3_sub(ray.origin, cylinder->center);
	radius = cylinder->diameter / 2.0;
	ray_axis_cross = vec3_cross(ray.direction, cylinder->axis);
	oc_axis_cross = vec3_cross(oc, cylinder->axis);
	q.a = vec3_dot(ray_axis_cross, ray_axis_cross);
	q.b = 2.0 * vec3_dot(ray_axis_cross, oc_axis_cross);
	q.c = vec3_dot(oc_axis_cross, oc_axis_cross) - radius * radius;
	return (q);
}

/*
** Calculate intersection with plane
** Returns 1 if hit, 0 if no hit
*/
int	intersect_plane(const t_plane *plane, t_ray ray, t_hit *hit)
{
	double	denom;
	double	t;
	t_vec3	oc;

	denom = vec3_dot(plane->normal, ray.direction);
	if (fabs(denom) < 0.0001)
		return (0);
	oc = vec3_sub(plane->point, ray.origin);
	t = vec3_dot(oc, plane->normal) / denom;
	if (t > 0.001)
	{
		if (hit->t < 0 || t < hit->t)
		{
			hit->t = t;
			hit->point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
			hit->normal = plane->normal;
			hit->color = plane->color;
			hit->obj_type = PLANE;
			hit->hit_side = -1;
			return (1);
		}
	}
	return (0);
}
