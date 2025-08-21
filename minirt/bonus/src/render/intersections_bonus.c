/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:19:05 by isallali          #+#    #+#             */
/*   Updated: 2025/08/21 21:19:06 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt_app_bonus.h"
#include "../includes/scene_bonus.h"

/*
** Compute quadratic coefficients for ray-sphere intersection
** Returns coefficients in t_quadratic struct
*/
static t_quadratic	sphere_quadratic_coeffs(const t_sphere *sphere, t_ray ray)
{
	t_quadratic	coeffs;
	t_vec3		oc;

	oc = vec3_sub(ray.origin, sphere->center);
	coeffs.a = vec3_dot(ray.direction, ray.direction);
	coeffs.b = 2.0 * vec3_dot(oc, ray.direction);
	coeffs.c = vec3_dot(oc, oc) - sphere->radius * sphere->radius;
	return (coeffs);
}

/*
** Calculate basic hit information for a sphere
*/
static int	calculate_sphere_hit(const t_sphere *sphere, t_ray ray, t_hit *hit)
{
	t_quadratic	coeffs;
	double		t;
	t_vec3		center_to_hit;
	double		phi;
	double		theta;

	coeffs = sphere_quadratic_coeffs(sphere, ray);
	if (coeffs.b * coeffs.b < 4.0 * coeffs.a * coeffs.c)
		return (0);
	t = solve_quadratic(coeffs.a, coeffs.b, coeffs.c, 0.001);
	if (t < 0.0)
		return (0);
	if (hit->t > 0.0 && t >= hit->t)
		return (0);
	hit->t = t;
	hit->point = vec3_add(ray.origin, vec3_mult(ray.direction, t));
	center_to_hit = vec3_sub(hit->point, sphere->center);
	hit->normal = vec3_normalize(center_to_hit);
	phi = atan2(hit->normal.z, hit->normal.x);
	theta = acos(hit->normal.y);
	hit->uv.u = (phi + M_PI) / (2.0 * M_PI);
	hit->uv.v = theta / M_PI;
	return (1);
}

int	intersect_sphere(const t_sphere *sphere, t_ray ray, t_hit *hit)
{
	if (!calculate_sphere_hit(sphere, ray, hit))
		return (0);
	hit->normal = apply_sphere_bump(sphere, hit);
	hit->color = determine_sphere_color(sphere, hit);
	hit->obj_type = SPHERE;
	hit->hit_side = -1;
	if (vec3_dot(ray.direction, hit->normal) > 0.0)
		hit->normal = vec3_mult(hit->normal, -1.0);
	return (1);
}

static int	trace_object(const t_object *obj, t_ray ray, t_hit *closest_hit,
		int index)
{
	int	hit;

	hit = 0;
	if (obj->type == SPHERE)
		hit = intersect_sphere(&obj->data.sphere, ray, closest_hit);
	else if (obj->type == PLANE)
		hit = intersect_plane(&obj->data.plane, ray, closest_hit);
	else if (obj->type == CYLINDER)
		hit = intersect_cylinder(&obj->data.cylinder, ray, closest_hit);
	else if (obj->type == CONE)
		hit = intersect_cone(&obj->data.cone, ray, closest_hit);
	if (hit)
		closest_hit->obj_index = index;
	return (hit);
}

int	trace_objects(const t_scene *scene, t_ray ray, t_hit *closest_hit)
{
	int		i;
	int		hit_found;
	t_hit	temp_hit;
	double	closest_distance;

	hit_found = 0;
	closest_distance = INT_MAX;
	if (scene->num_objects == 0)
		return (0);
	i = 0;
	while (i < scene->num_objects)
	{
		temp_hit.t = -1.0;
		if (trace_object(&scene->objects[i], ray, &temp_hit, i))
		{
			if (temp_hit.t > 0.001 && temp_hit.t < closest_distance)
			{
				*closest_hit = temp_hit;
				closest_distance = temp_hit.t;
				hit_found = 1;
			}
		}
		i++;
	}
	return (hit_found);
}
