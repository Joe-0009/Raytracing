#include "../includes/color_utils.h"
#include "../includes/intersections.h"
#include "../includes/main.h"
#include "../includes/math_utils.h"
#include "../includes/raytrace.h"
#include "../includes/scene.h"
#include <math.h>

static t_vec3	get_camera_up_vector(t_vec3 forward)
{
	t_vec3	world_up;

	world_up = vec3_create(0, 1, 0);
	if (fabs(vec3_dot(forward, world_up)) > 0.99)
		world_up = vec3_create(0, 0, 1);
	return (world_up);
}

static void	get_camera_basis(const t_scene *scene, t_vec3 *forward, t_vec3 *right, t_vec3 *up)
{
	t_vec3	world_up;

	*forward = vec3_normalize(scene->camera.orientation);
	world_up = get_camera_up_vector(*forward);
	*right = vec3_normalize(vec3_cross(*forward, world_up));
	*up = vec3_cross(*right, *forward);
}

t_ray	generate_camera_ray(const t_scene *scene, int x, int y)
{
	t_ray	ray;
	double	scale;
	double	image_x;
	double	image_y;
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;

	scale = tan(scene->camera.fov * M_PI / 360.0);
	image_x = (2.0 * ((double)x + 0.5) / WIDTH - 1.0) * scale * WIDTH / HEIGHT;
	image_y = (1.0 - 2.0 * ((double)y + 0.5) / HEIGHT) * scale;
	
	get_camera_basis(scene, &forward, &right, &up);
	
	ray.origin = scene->camera.position;
	ray.direction = vec3_normalize(vec3_add(vec3_add(
		vec3_mult(right, image_x), vec3_mult(up, image_y)), forward));
	
	return (ray);
}

double	hit_sphere(const t_sphere *sphere, t_ray ray)
{
	t_quadratic	q;

	q = sphere_quadratic_coeffs(sphere, ray);
	return (solve_quadratic(q.a, q.b, q.c, 0.001));
}

static double	intersect_cylinder_cap(const t_cylinder *cylinder, t_ray ray, int is_top)
{
	t_vec3	cap_center;
	t_vec3	cap_normal;
	double	denom;
	double	t;
	t_vec3	p;

	cap_center = cylinder->center;
	if (is_top)
	{
		cap_center = vec3_add(cylinder->center, 
			vec3_mult(cylinder->axis, cylinder->height));
		cap_normal = cylinder->axis;
	}
	else
		cap_normal = vec3_mult(cylinder->axis, -1.0);
	denom = vec3_dot(ray.direction, cap_normal);
	if (fabs(denom) < 1e-6 || (t = vec3_dot(vec3_sub(cap_center, ray.origin), 
		cap_normal) / denom) < 0.001)
		return (-1);
	p = vec3_add(ray.origin, vec3_mult(ray.direction, t));
	if (vec3_length(vec3_sub(p, cap_center)) > cylinder->diameter / 2.0)
		return (-1);
	return (t);
}

static double	get_closest_cylinder_hit(double t_side, double t_cap0, double t_cap1)
{
	double	t;

	t = -1;
	if (t_side > 0)
		t = t_side;
	if (t_cap0 > 0 && (t < 0 || t_cap0 < t))
		t = t_cap0;
	if (t_cap1 > 0 && (t < 0 || t_cap1 < t))
		t = t_cap1;
	return (t);
}

static void	update_closest_hit(double new_t, int new_type, double *closest_t, int *hit_type)
{
	if (new_t > 0 && (*closest_t < 0 || new_t < *closest_t))
	{
		*closest_t = new_t;
		*hit_type = new_type;
	}
}

double	hit_cylinder(const t_cylinder *cylinder, t_ray ray)
{
	t_quadratic	q;
	double		t_side;
	double		t_cap0;
	double		t_cap1;
	t_vec3		d_perp;
	t_vec3		oc_perp;

	q = cylinder_quadratic_coeffs(cylinder, ray, &d_perp, &oc_perp);
	t_side = solve_quadratic(q.a, q.b, q.c, 0.001);
	if (t_side > 0 && !cylinder_height_check(cylinder, ray, t_side))
		t_side = -1;
	t_cap0 = intersect_cylinder_cap(cylinder, ray, 0);
	t_cap1 = intersect_cylinder_cap(cylinder, ray, 1);
	return (get_closest_cylinder_hit(t_side, t_cap0, t_cap1));
}

double	hit_cylinder_with_type(const t_cylinder *cylinder, t_ray ray, int *hit_type)
{
	t_quadratic	q;
	double		t_side;
	double		t_cap0;
	double		t_cap1;
	double		t;
	t_vec3		d_perp;
	t_vec3		oc_perp;

	t = -1;
	*hit_type = -1;
	q = cylinder_quadratic_coeffs(cylinder, ray, &d_perp, &oc_perp);
	t_side = solve_quadratic(q.a, q.b, q.c, 0.001);
	if (t_side > 0 && cylinder_height_check(cylinder, ray, t_side))
		update_closest_hit(t_side, 2, &t, hit_type);
	t_cap0 = intersect_cylinder_cap(cylinder, ray, 0);
	update_closest_hit(t_cap0, 0, &t, hit_type);
	t_cap1 = intersect_cylinder_cap(cylinder, ray, 1);
	update_closest_hit(t_cap1, 1, &t, hit_type);
	return (t);
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
