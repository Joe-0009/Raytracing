#include "../includes/minirt_app_bonus.h"
#include "../includes/scene_bonus.h"
#include <math.h>

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
	double	phi;
	double	theta;

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
	/* cache UV once per hit */
	phi = atan2(hit->normal.z, hit->normal.x);
	theta = acos(hit->normal.y);
	hit->uv.u = (phi + M_PI) / (2.0 * M_PI);
	hit->uv.v = theta / M_PI;
	return (1);
}


/*
** Sample a color from a texture at given UV
*/
t_color3	sample_texture(const t_surface_map *texture, t_uv uv)
{
	int			idx;
	int x;
	int y;
	t_color3 color;
	
	color.x= 0;
	color.y= 0;
	color.z= 0;
	if (!texture->is_active || !texture->data)
		return (color);
	uv.u = fmod(uv.u + texture->rotation_uv.u, 1.0);
	x = (int)(uv.u * (texture->width - 1));
	y = (int)(uv.v * (texture->height - 1));
	idx = (y * texture->width + x) * 3;
	color.x = texture->data[idx] / 255.0;
	color.y = texture->data[idx + 1] / 255.0;
	color.z = texture->data[idx + 2] / 255.0;
	return (color);
}
/*
** Sample a grayscale value from a bump map at given UV
*/
double sample_bump_map(const t_surface_map *bump, t_uv uv)
{
	int idx;
	double value;
	int x;
	int y;

	if (!bump->is_active || !bump->data)
		return 0.0;
	uv.u = fmod(uv.u + bump->rotation_uv.u, 1.0);
	x = (int)(uv.u * (bump->width - 1));
	y = (int)(uv.v * (bump->height - 1));
	idx = (y * bump->width + x) * 3;
	value = bump->data[idx];
	return (value  / 255.0);
}


/*
** Apply bump mapping to a surface normal using the bump map
*/
t_vec3 apply_sphere_bump(const t_sphere *sphere, t_hit *hit)
{
	double  h_center, h_right, h_up, du, dv;
	t_uv uv_right, uv_up;
	t_vec3 bumped;
	t_vec3 tangent, bitangent;

	if (!sphere->bump.is_active || !sphere->bump.data) {
		return hit->normal;
	}
	
	tangent = vec3_normalize(vec3_create(-hit->normal.z, 0, hit->normal.x));
	bitangent = vec3_normalize(vec3_cross(hit->normal, tangent));

	h_center = sample_bump_map(&sphere->bump, hit->uv);
	
	uv_right = hit->uv;
	uv_right.u += 1.0 / sphere->bump.width;
	h_right = sample_bump_map(&sphere->bump, uv_right);

	uv_up = hit->uv;
	uv_up.v += 1.0 / sphere->bump.height;
	h_up = sample_bump_map(&sphere->bump, uv_up);

	du = (h_right - h_center) * 10.0;
	dv = (h_up - h_center) * 10.0;

	bumped = vec3_add(hit->normal,
				vec3_add(
					vec3_mult(tangent, du),
					vec3_mult(bitangent, dv)
				));
	return vec3_normalize(bumped);
}

/*
** Determine color at hit point
*/
static t_vec3	determine_sphere_color(const t_sphere *sphere, t_hit *hit)
{
	int	check_u;
	int	check_v;

	if (sphere->texture.is_active)
		return (sample_texture(&sphere->texture, hit->uv));
	if (sphere->checkerboard)
	{
		check_u = (int)floor(hit->uv.u * 10.0);
		check_v = (int)floor(hit->uv.v * 10.0);
		if ((check_u + check_v) % 2 == 0)
			return (vec3_create(1.0, 1.0, 1.0));
		return (vec3_create(0.0, 0.0, 0.0));
	}
	return (sphere->color);
}
 
/*
** Full sphere intersection entry point
*/
int	intersect_sphere(const t_sphere *sphere, t_ray ray, t_hit *hit)
{
	if (!calculate_sphere_hit(sphere, ray, hit))
		return (0);

	hit->normal = apply_sphere_bump(sphere, hit);
	hit->color  = determine_sphere_color(sphere, hit);
	hit->obj_type = SPHERE;
	hit->hit_side = -1;

	if (vec3_dot(ray.direction, hit->normal) > 0.0)
		hit->normal = vec3_mult(hit->normal, -1.0);
	return (1);
}
