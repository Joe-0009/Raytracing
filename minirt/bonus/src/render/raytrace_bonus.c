#include "../includes/constants_bonus.h"
#include "../includes/events_bonus.h"
#include "../includes/minirt_app_bonus.h"
#include "../includes/render_utils_bonus.h"
#include "../includes/scene_bonus.h"
#include <math.h>

/*
** Generate a camera ray for a given pixel (x, y)
*/
t_ray	generate_camera_ray(const t_scene *scene, int x, int y)
{
	t_ray				ray;
	t_camera_vectors	camera_vectors;
	double				pixel_scale;
	double				u;
	double				v;

	ray.origin = scene->camera.position;
	camera_vectors.forward = vec3_normalize(scene->camera.orientation);
	camera_vectors.right = vec3_normalize(vec3_cross(camera_vectors.forward,
				vec3_create(0, 1, 0)));
	camera_vectors.up = vec3_cross(camera_vectors.right,
			camera_vectors.forward);
	pixel_scale = tan((scene->camera.fov * M_PI / 180.0) / 2.0) / (WIDTH / 2.0);
	u = (x - WIDTH / 2.0) * pixel_scale;
	v = (HEIGHT / 2.0 - y) * pixel_scale;
	ray.direction = vec3_normalize(vec3_add(vec3_add(vec3_mult(camera_vectors.right,
						u), vec3_mult(camera_vectors.up, v)),
				vec3_normalize(scene->camera.orientation)));
	return (ray);
}

/*
** Apply selection highlighting to color
*/
static t_color3	apply_selection_highlight(t_color3 color)
{
	color.x = color.x + (1.0 - color.x) * LIGHTENING_FACTOR;
	color.y = color.y + (1.0 - color.y) * LIGHTENING_FACTOR;
	color.z = color.z + (1.0 - color.z) * LIGHTENING_FACTOR;
	return (color);
}

/*
** Trace a ray and return the color for the pixel
*/
int	trace_ray(const t_scene *scene, t_ray ray)
{
	t_hit		closest_hit;
	t_color3	final_color;
	t_vec3		view_dir;

	if (trace_objects(scene, ray, &closest_hit))
	{
		view_dir = vec3_mult(ray.direction, -1.0);
		view_dir = vec3_normalize(view_dir);
		final_color = calculate_phong_lighting(scene, &closest_hit, view_dir);
		if (closest_hit.obj_index == scene->selected_obj)
			final_color = apply_selection_highlight(final_color);
		return (color_to_int(final_color));
	}
	return (get_sky_color(ray));
}
