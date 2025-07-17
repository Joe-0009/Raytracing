#include "../includes/scene_bonus.h"
#include <math.h>
#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
** Convert UV coordinates to pixel coordinates with normalization
*/
static void	uv_to_normalized_pixel_coords(const t_surface_map *map, t_uv uv,
		int *x, int *y)
{
	uv.u = uv.u - floor(uv.u);
	uv.v = uv.v - floor(uv.v);
	*x = (int)(uv.u * (map->width - 1));
	*y = (int)(uv.v * (map->height - 1));
	if (*x >= map->width)
		*x = map->width - 1;
	if (*y >= map->height)
		*y = map->height - 1;
}

/*
** Apply texture rotation based on surface map rotation settings
*/
static t_uv	apply_texture_rotation(const t_surface_map *map, t_uv uv)
{
	double	u_offset;
	double	v_offset;

	if (!map->is_active)
		return (uv);
	u_offset = map->rotation_uv.u;
	uv.u = fmod(uv.u + u_offset, 1.0);
	if (uv.u < 0.0)
		uv.u += 1.0;
	v_offset = map->rotation_uv.v;
	uv.v = fmod(uv.v + v_offset, 1.0);
	if (uv.v < 0.0)
		uv.v += 1.0;
	return (uv);
}

/*
** Sample bump map at UV coordinates
*/
double	sample_bump_map(const t_surface_map *bump, t_uv uv)
{
	int		index;
	double	height;
	int		x;
	int		y;
	t_uv	rotated_uv;

	rotated_uv = apply_texture_rotation(bump, uv);
	uv_to_normalized_pixel_coords(bump, rotated_uv, &x, &y);
	index = (y * bump->width + x) * 3;
	height = (bump->data[index] * 0.299 + bump->data[index + 1] * 0.587
			+ bump->data[index + 2] * 0.114) / 255.0;
	return (height * 2.0 - 1.0);
}

/*
** Sample texture at UV coordinates
*/
t_color3	sample_texture(const t_surface_map *texture, t_uv uv)
{
	int			index;
	t_color3	color;
	int			x;
	int			y;
	t_uv		rotated_uv;

	rotated_uv = apply_texture_rotation(texture, uv);
	uv_to_normalized_pixel_coords(texture, rotated_uv, &x, &y);
	index = (y * texture->width + x) * 3;
	color.x = texture->data[index] / 255.0;
	color.y = texture->data[index + 1] / 255.0;
	color.z = texture->data[index + 2] / 255.0;
	return (color);
}

/*
** Apply bump mapping to surface normal
*/
t_vec3	apply_bump_mapping(t_vec3 normal, t_uv uv, const t_surface_map *bump,
		t_vec3 tangent, t_vec3 bitangent)
{
	double	bump_scale;

	double h_center, h_right, h_up;
	double du, dv;
	t_uv uv_right, uv_up;
	bump_scale = 2.0;
	h_center = sample_bump_map(bump, uv);
	uv_right = uv;
	uv_right.u += 1.0 / bump->width;
	h_right = sample_bump_map(bump, uv_right);
	uv_up = uv;
	uv_up.v += 1.0 / bump->height;
	h_up = sample_bump_map(bump, uv_up);
	du = (h_right - h_center) * bump_scale * 5.0;
	dv = (h_up - h_center) * bump_scale * 5.0;
	return (vec3_normalize(vec3_add(vec3_add(normal, vec3_mult(tangent, du)),
				vec3_mult(bitangent, dv))));
}

/*
** Calculate UV coordinates for sphere mapping
*/
t_uv	sphere_uv_mapping(t_vec3 normalized)
{
	t_uv	uv;

	double phi, theta;
	if (normalized.y > 1.0)
		normalized.y = 1.0;
	else if (normalized.y < -1.0)
		normalized.y = -1.0;
	phi = atan2(normalized.z, normalized.x);
	theta = acos(normalized.y);
	uv.u = (phi + M_PI) / (2.0 * M_PI);
	uv.v = theta / M_PI;
	return (uv);
}

void	increment_texture_rotation(t_surface_map *map, int axis, double delta)
{
	if (!map || !map->is_active)
		return ;
	if (axis == 0)
		map->rotation_uv.u += delta;
	else if (axis == 1)
		map->rotation_uv.v += delta;
	if (map->rotation_uv.u > 1.0)
		map->rotation_uv.u -= 1.0;
	if (map->rotation_uv.u < 0.0)
		map->rotation_uv.u += 1.0;
	if (map->rotation_uv.v > 1.0)
		map->rotation_uv.v -= 1.0;
	if (map->rotation_uv.v < 0.0)
		map->rotation_uv.v += 1.0;
}

void	increment_sphere_texture_rotation(t_sphere *sphere, int axis,
		double delta)
{
	if (sphere->texture.is_active)
		increment_texture_rotation(&sphere->texture, axis, delta);
	if (sphere->bump.is_active)
		increment_texture_rotation(&sphere->bump, axis, delta);
}
