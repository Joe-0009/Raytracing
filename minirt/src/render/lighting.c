#include "../includes/lighting.h"
#include <math.h>

/*
 * Calculate ambient lighting component
 * Formula: ambient_ratio × ambient_color × object_color
 */
t_color3	calculate_ambient(const t_scene *scene, const t_hit *hit)
{
	t_color3	ambient;

	ambient.x = scene->ambient.ratio * scene->ambient.color.x * hit->color.x;
	ambient.y = scene->ambient.ratio * scene->ambient.color.y * hit->color.y;
	ambient.z = scene->ambient.ratio * scene->ambient.color.z * hit->color.z;
	return (ambient);
}

/*
 * Calculate diffuse lighting component
 * Formula: light_intensity × light_color × object_color × max(0, dot(normal, light_dir))
 */
t_color3	calculate_diffuse(const t_scene *scene, const t_hit *hit)
{
	t_color3	diffuse;
	t_vec3		light_dir;
	double		dot_product;
	double		distance;
	double		attenuation;

	/* Calculate direction from hit point to light source */
	light_dir = vec3_sub(scene->light.position, hit->point);
	distance = vec3_length(light_dir);
	light_dir = vec3_normalize(light_dir);
	
	/* Calculate how much the surface faces the light (Lambert's cosine law) */
	dot_product = vec3_dot(hit->normal, light_dir);
	
	/* Only apply diffuse lighting if surface faces the light */
	if (dot_product < 0.0)
		dot_product = 0.0;
	
	/* Check if the point is in shadow */
	if (is_in_shadow(scene, hit->point, scene->light.position))
	{
		/* In shadow - no diffuse lighting */
		diffuse.x = 0.0;
		diffuse.y = 0.0;
		diffuse.z = 0.0;
		return (diffuse);
	}
	
	/* Optional: Apply distance attenuation (light gets weaker with distance) */
	attenuation = 1.0 / (1.0 + 0.01 * distance + 0.001 * distance * distance);
	
	/* Calculate diffuse color */
	diffuse.x = scene->light.brightness * scene->light.color.x * hit->color.x * dot_product * attenuation;
	diffuse.y = scene->light.brightness * scene->light.color.y * hit->color.y * dot_product * attenuation;
	diffuse.z = scene->light.brightness * scene->light.color.z * hit->color.z * dot_product * attenuation;
	
	return (diffuse);
}

/*
 * Check if a point is in shadow from a light source
 * Returns 1 if in shadow, 0 if illuminated
 */
int	is_in_shadow(const t_scene *scene, const t_vec3 point, const t_vec3 light_pos)
{
	t_ray		shadow_ray;
	t_hit		shadow_hit;
	t_vec3		light_dir;
	double		light_distance;
	double		epsilon;

	/* Small offset to avoid self-intersection (shadow acne) */
	epsilon = 1e-6;
	
	/* Calculate direction and distance to light */
	light_dir = vec3_sub(light_pos, point);
	light_distance = vec3_length(light_dir);
	light_dir = vec3_normalize(light_dir);
	
	/* Create shadow ray from hit point towards light */
	shadow_ray.origin = vec3_add(point, vec3_mult(light_dir, epsilon));
	shadow_ray.direction = light_dir;
	
	/* Check if any object intersects the shadow ray before reaching the light */
	if (trace_objects(scene, shadow_ray, &shadow_hit))
	{
		/* If intersection distance is less than light distance, we're in shadow */
		if (shadow_hit.t < light_distance - epsilon)
			return (1);
	}
	
	return (0);
}

/*
 * Main lighting calculation function
 * Currently only implements ambient lighting
 * Later we'll add diffuse and specular components here
 */
t_color3	calculate_lighting(const t_scene *scene, const t_hit *hit)
{
	t_color3	final_color;

	/* Calculate ambient component */
	final_color = calculate_ambient(scene, hit);
	
	/* Calculate diffuse component */
	final_color = vec3_add(final_color, calculate_diffuse(scene, hit));
	
	/* TODO: Add specular lighting here later */
	
	/* Clamp final color to valid range */
	final_color = clamp_color(final_color);
	
	return (final_color);
}


