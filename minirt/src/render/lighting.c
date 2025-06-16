#include "../includes/minirt_app.h"
#include "../includes/constants.h"
#include <math.h>

/*
** Calculate diffuse lighting component with attenuation
** Formula: light_intensity × light_color × object_color × max(0, dot(normal, light_dir)) × attenuation
** where attenuation = 1.0 / (1.0 + ATTENUATION_LINEAR * distance + ATTENUATION_QUADRATIC * distance²)
*/
t_color3	calculate_diffuse(const t_scene *scene, const t_hit *hit)
{
	t_vec3		light_dir;
	double		dot_product;
	double		distance;
	double		attenuation;
	t_color3	diffuse;

	light_dir = vec3_sub(scene->light.position, hit->point);
	distance = vec3_length(light_dir);
	light_dir = vec3_normalize(light_dir);
	dot_product = vec3_dot(hit->normal, light_dir);
	if (dot_product < 0.0)
		dot_product = 0.0;
	if (is_in_shadow(scene, hit->point, scene->light.position))
		return (vec3_create(0.0, 0.0, 0.0));
	attenuation = 1.0 / (1.0 + ATTENUATION_LINEAR * distance + ATTENUATION_QUADRATIC * distance * distance);
	diffuse.x = scene->light.brightness * scene->light.color.x * hit->color.x * dot_product * attenuation;
	diffuse.y = scene->light.brightness * scene->light.color.y * hit->color.y * dot_product * attenuation;
	diffuse.z = scene->light.brightness * scene->light.color.z * hit->color.z * dot_product * attenuation;
	return (diffuse);
}

/*
** Check if a point is in shadow from a light source
** Returns 1 if in shadow, 0 if illuminated
** Also handles shadow ray setup internally
*/
int	is_in_shadow(const t_scene *scene, const t_vec3 point,
		const t_vec3 light_pos)
{
	t_ray	shadow_ray;
	t_hit	shadow_hit;
	t_vec3	light_dir;
	double	light_distance;

	light_dir = vec3_sub(light_pos, point);
	light_distance = vec3_length(light_dir);
	light_dir = vec3_normalize(light_dir);
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
** Main lighting calculation function
** Combines ambient and diffuse lighting components
** Formula: ambient + diffuse, where:
** - ambient = ambient_ratio × ambient_color × object_color
** - diffuse = light_intensity × light_color × object_color × max(0, dot(normal, light_dir)) × attenuation
*/
t_color3	calculate_lighting(const t_scene *scene, const t_hit *hit)
{
	t_color3	final_color;
	t_color3	ambient;

	ambient.x = scene->ambient.ratio * scene->ambient.color.x * hit->color.x;
	ambient.y = scene->ambient.ratio * scene->ambient.color.y * hit->color.y;
	ambient.z = scene->ambient.ratio * scene->ambient.color.z * hit->color.z;


	final_color = vec3_add(ambient, calculate_diffuse(scene, hit));
	final_color = clamp_color(final_color);
	return (final_color);
}
