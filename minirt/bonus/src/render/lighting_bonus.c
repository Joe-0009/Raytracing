/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:31:34 by isallali          #+#    #+#             */
/*   Updated: 2025/08/21 21:31:35 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/constants_bonus.h"
#include "../includes/minirt_app_bonus.h"
#include "../includes/render_utils_bonus.h"
#include <math.h>

/*
** Check if a point is in shadow from a light source
** using precomputed light direction
** Returns 1 if in shadow, 0 if illuminated
*/
int	is_in_shadow(const t_scene *scene, const t_vec3 point,
		const t_vec3 light_dir, double light_distance)
{
	t_ray	shadow_ray;
	t_hit	shadow_hit;

	shadow_ray.origin = vec3_add(point, vec3_mult(light_dir, SHADOW_EPSILON));
	shadow_ray.direction = light_dir;
	if (trace_objects(scene, shadow_ray, &shadow_hit))
	{
		if (shadow_hit.t < light_distance - SHADOW_EPSILON)
			return (1);
	}
	return (0);
}

/*
** Calculate precomputed light data for a single light source
** This eliminates redundant calculations between diffuse and specular
*/
t_light_data	calculate_light_data(const t_light *light, const t_hit *hit,
		const t_scene *scene)
{
	t_light_data	data;

	data.light_dir = vec3_sub(light->position, hit->point);
	data.distance = vec3_length(data.light_dir);
	data.light_dir = vec3_normalize(data.light_dir);
	data.normal_dot_light = vec3_dot(hit->normal, data.light_dir);
	data.in_shadow = is_in_shadow(scene, hit->point, data.light_dir,
			data.distance);
	data.attenuation = 1.0 / (1.0 + ATTENUATION_LINEAR * data.distance
			+ ATTENUATION_QUADRATIC * data.distance * data.distance);
	return (data);
}

/*
** Calculate diffuse lighting component using precomputed light data
** Formula: light_intensity × light_color × object_color × max(0, dot(normal,
		light_dir)) × attenuation
*/
t_color3	calculate_diffuse_with_data(const t_light *light, const t_hit *hit,
		const t_light_data *data)
{
	t_color3	diffuse;
	double		dot_product;

	dot_product = data->normal_dot_light;
	if (dot_product < 0.0)
		dot_product = 0.0;
	if (data->in_shadow)
		return (vec3_create(0.0, 0.0, 0.0));
	diffuse.x = light->brightness * light->color.x * hit->color.x * dot_product
		* data->attenuation;
	diffuse.y = light->brightness * light->color.y * hit->color.y * dot_product
		* data->attenuation;
	diffuse.z = light->brightness * light->color.z * hit->color.z * dot_product
		* data->attenuation;
	return (diffuse);
}

/*
** Calculate specular reflection component using precomputed light data
** Formula: light_intensity × light_color × max(0, dot(reflect_dir,
		view_dir))^shininess × attenuation
** where reflect_dir = reflect(-light_dir, normal)
*/
t_color3	calculate_specular_with_data(const t_light *light, const t_hit *hit,
		const t_vec3 view_dir, const t_light_data *data)
{
	t_vec3		reflect_dir;
	double		spec_dot;
	t_color3	specular;

	if (data->normal_dot_light <= 0.0)
		return (vec3_create(0.0, 0.0, 0.0));
	if (data->in_shadow)
		return (vec3_create(0.0, 0.0, 0.0));
	reflect_dir = reflect(vec3_mult(data->light_dir, -1.0), hit->normal);
	reflect_dir = vec3_normalize(reflect_dir);
	spec_dot = vec3_dot(reflect_dir, view_dir);
	if (spec_dot <= 0.0)
		return (vec3_create(0.0, 0.0, 0.0));
	spec_dot = pow(spec_dot, SPECULAR_SHININESS);
	specular.x = light->brightness * light->color.x * SPECULAR_INTENSITY
		* spec_dot * data->attenuation;
	specular.y = light->brightness * light->color.y * SPECULAR_INTENSITY
		* spec_dot * data->attenuation;
	specular.z = light->brightness * light->color.z * SPECULAR_INTENSITY
		* spec_dot * data->attenuation;
	return (specular);
}

/*
** Calculate both diffuse and specular lighting components efficiently
** This eliminates redundant calculations by processing each light once
*/
t_color3	calculate_phong_lighting(const t_scene *scene, const t_hit *hit,
		const t_vec3 view_dir)
{
	int				i;
	t_light_data	light_data;

	t_color3
	(final_color), (ambient), (total_diffuse), (total_specular);
	total_diffuse = vec3_create(0.0, 0.0, 0.0);
	total_specular = vec3_create(0.0, 0.0, 0.0);
	i = -1;
	while (++i < scene->nbr_of_lights)
	{
		light_data = calculate_light_data(&scene->light[i], hit, scene);
		total_diffuse = vec3_add(total_diffuse,
				calculate_diffuse_with_data(&scene->light[i], hit,
					&light_data));
		total_specular = vec3_add(total_specular,
				calculate_specular_with_data(&scene->light[i], hit, view_dir,
					&light_data));
	}
	ambient.x = scene->ambient.ratio * scene->ambient.color.x * hit->color.x;
	ambient.y = scene->ambient.ratio * scene->ambient.color.y * hit->color.y;
	ambient.z = scene->ambient.ratio * scene->ambient.color.z * hit->color.z;
	final_color = vec3_add(vec3_add(ambient, total_diffuse), total_specular);
	final_color = clamp_color(final_color);
	return (final_color);
}
