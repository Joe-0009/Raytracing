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
	t_vec3	forward;
	t_vec3	world_up;
	t_vec3	right;
	t_vec3	up;
	double	fov_radians;
	double	pixel_scale;
	double	u;
	double	v;

	ray.origin = scene->camera.position;
	forward = vec3_normalize(scene->camera.orientation);
	world_up = vec3_create(0, 1, 0);
	right = vec3_normalize(vec3_cross(forward, world_up));
	up = vec3_cross(right, forward);
	fov_radians = scene->camera.fov * M_PI / 180.0;
	pixel_scale = tan(fov_radians / 2.0) / (WIDTH / 2.0);
	u = (x - WIDTH / 2.0) * pixel_scale;
	v = (HEIGHT / 2.0 - y) * pixel_scale;
	ray.direction = vec3_normalize(vec3_add(
						vec3_add(
							vec3_mult(right, u),
							vec3_mult(up, v)
						),
						forward
					));
	return (ray);
}

int	trace_ray(const t_scene *scene, t_ray ray)
{
	t_hit	closest_hit;

	if (trace_objects(scene, ray, &closest_hit))
		return (color_to_int(closest_hit.color));
	return (get_sky_color(ray));
}
