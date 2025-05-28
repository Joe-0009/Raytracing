#include "../includes/color_utils.h"
#include "../includes/intersections.h"
#include "../includes/main.h"
#include "../includes/math_utils.h"
#include "../includes/raytrace.h"
#include "../includes/scene.h"
#include <math.h>

t_ray	generate_camera_ray(const t_scene *scene, int x, int y)
{
	t_ray	ray;
	t_vec3 forward;
	t_vec3 up;
	t_vec3 right;

	forward = vec3_normalize(vec3_sub(scene->camera.orientation, scene->camera.position));
	up = vec3_create(0, 1, 0);
	right = vec3_normalize(vec3_cross(forward, up));
	ray.origin = scene->camera.position;
	ray.direction = vec3_normalize(vec3_add(scene->camera.orientation,
				vec3_create(u * 0.003, v * 0.003, 0)));
	return (ray);
}

double	hit_sphere(const t_sphere *sphere, t_ray ray)
{
	t_quadratic	q;

	q = sphere_quadratic_coeffs(sphere, ray);
	return (solve_quadratic(q.a, q.b, q.c, 0.001));
}

int	trace_ray(const t_scene *scene, t_ray ray)
{
	t_hit	closest_hit;

	if (trace_objects(scene, ray, &closest_hit))
	{
		return (color_to_int(closest_hit.color));
	}
	return (get_sky_color(ray));
}
