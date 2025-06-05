#include "../includes/events.h"
#include "../includes/minirt_app.h"
#include "../includes/scene_math.h"
#include <math.h>

/*
** Calculate camera basis vectors
*/
static void	calculate_camera_basis(const t_scene *scene, t_vec3 *right,
				t_vec3 *up)
{
	t_vec3	forward;
	t_vec3	world_up;

	forward = vec3_normalize(scene->camera.orientation);
	world_up = vec3_create(0, 1, 0);
	*right = vec3_normalize(vec3_cross(forward, world_up));
	*up = vec3_cross(*right, forward);
}

/*
** Generate a camera ray for a given pixel (x, y)
*/
t_ray	generate_camera_ray(const t_scene *scene, int x, int y)
{
	t_ray	ray;
	t_vec3	right;
	t_vec3	up;
	double	pixel_scale;
	double	u;

	ray.origin = scene->camera.position;
	calculate_camera_basis(scene, &right, &up);
	pixel_scale = tan((scene->camera.fov * M_PI / 180.0) / 2.0)
		/ (WIDTH / 2.0);
	u = (x - WIDTH / 2.0) * pixel_scale;
	ray.direction = vec3_normalize(vec3_add(
		vec3_add(vec3_mult(right, u),
			vec3_mult(up, (HEIGHT / 2.0 - y) * pixel_scale)),
		vec3_normalize(scene->camera.orientation)));
	return (ray);
}

/*
** Get the currently selected object index
*/
int	get_selected_object_index(void)
{
	return (g_selected_obj);
}

/*
** Trace a ray and return the color for the pixel
*/
int	trace_ray(const t_scene *scene, t_ray ray)
{
	t_hit		closest_hit;
	t_color3	final_color;
	double		lightening_factor;

	if (trace_objects(scene, ray, &closest_hit))
	{
		final_color = calculate_lighting(scene, &closest_hit);
		if (closest_hit.obj_index == get_selected_object_index())
		{
			lightening_factor = 0.4;
			final_color.x = final_color.x + (1.0 - final_color.x)
				* lightening_factor;
			final_color.y = final_color.y + (1.0 - final_color.y)
				* lightening_factor;
			final_color.z = final_color.z + (1.0 - final_color.z)
				* lightening_factor;
		}
		return (color_to_int(final_color));
	}
	return (get_sky_color(ray));
}
