#include "../includes/scene_bonus.h"
#include <math.h>
#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
** Convert UV coordinates to pixel coordinates with normalization
** First normalizes UVs to [0, 1] range, then converts to pixel coordinates
*/
static void	uv_to_normalized_pixel_coords(const t_surface_map *map, t_uv uv,
		int *x, int *y)
{
	// Normalize UV coordinates to [0, 1] range with proper handling of negative values
	uv.u = uv.u - floor(uv.u);
	uv.v = uv.v - floor(uv.v);
	
	// Convert to pixel coordinates
	*x = (int)(uv.u * (map->width - 1));
	*y = (int)(uv.v * (map->height - 1));
	
	// Clamp to valid pixel range (safety check)
	if (*x < 0)
		*x = 0;
	else if (*x >= map->width)
		*x = map->width - 1;
	
	if (*y < 0)
		*y = 0;
	else if (*y >= map->height)
		*y = map->height - 1;
}

/*
** Rotate UV coordinates around the center (0.5, 0.5)
** angle is in radians
*/
static t_uv	rotate_uv(t_uv uv, double angle)
{
	t_uv	rotated;
	double	cos_angle;
	double	sin_angle;
	double	centered_u;
	double	centered_v;

	cos_angle = cos(angle);
	sin_angle = sin(angle);
	centered_u = uv.u - 0.5;
	centered_v = uv.v - 0.5;
	rotated.u = centered_u * cos_angle - centered_v * sin_angle + 0.5;
	rotated.v = centered_u * sin_angle + centered_v * cos_angle + 0.5;
	return (rotated);
}

/*
** Apply texture rotation based on surface map rotation settings
*/
static t_uv	apply_texture_rotation(const t_surface_map *map, t_uv uv)
{
	t_uv	rotated_uv;
	double	rotation_angle;

	if (!map->is_active)
		return (uv);
	rotation_angle = map->rotation_uv.u;
	if (fabs(rotation_angle) < 1e-6)
		return (uv);
	rotated_uv = rotate_uv(uv, rotation_angle);
	return (rotated_uv);
}

/*
** Sample bump map at UV coordinates
** Returns height value (0.0 to 1.0) from grayscale bump map
** Making the surface look bumpy (bump mapping)
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
	height = bump->data[index] / 255.0;
	height = (height - 0.5) * 2.0 + 0.5;
	if (height < 0.0)
		height = 0.0;
	if (height > 1.0)
		height = 1.0;
	return (height);
}

/*
** Sample texture at UV coordinates
** Reading the color from that spot (texture sampling)
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
** Uses the bump map to perturb the surface normal for realistic lighting
*/
t_vec3	apply_bump_mapping(t_vec3 normal, t_uv uv, const t_surface_map *bump,
		t_vec3 tangent, t_vec3 bitangent)
{
	double	bump_scale;
	t_vec3	bump_normal;
	t_vec3	deviation;

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
	du = du * (1.0 + fabs(h_center - 0.5) * 2.0);
	dv = dv * (1.0 + fabs(h_center - 0.5) * 2.0);
	bump_normal = vec3_add(normal, vec3_mult(tangent, du));
	bump_normal = vec3_add(bump_normal, vec3_mult(bitangent, dv));
	deviation = vec3_sub(bump_normal, normal);
	deviation = vec3_mult(deviation, 1.5);
	bump_normal = vec3_add(normal, deviation);
	return (vec3_normalize(bump_normal));
}

/*
** Calculate UV coordinates for sphere mapping using pre-calculated normalized vector
** Optimized version to avoid redundant calculations
*/
t_uv	sphere_uv_mapping(t_vec3 normalized)
{
	t_uv	uv;
	double phi, theta;
	
	phi = atan2(normalized.z, normalized.x);
	theta = acos(normalized.y);
	uv.u = (phi + M_PI) / (2.0 * M_PI);
	uv.v = theta / M_PI;
	return (uv);
}

/*
** Set texture rotation in degrees
*/
void	set_texture_rotation_degrees(t_surface_map *map, double angle_degrees)
{
	if (!map)
		return ;
	map->rotation_uv.u = angle_degrees * M_PI / 180.0;
	map->rotation_uv.v = 0.0; // Reserved for future use
}

/*
** Set texture rotation for a sphere (both texture and bump map)
*/
void	set_sphere_texture_rotation(t_sphere *sphere, double angle_degrees)
{
	if (!sphere)
		return ;
	
	// Apply rotation to both texture and bump map if active
	if (sphere->texture.is_active)
		set_texture_rotation_degrees(&sphere->texture, angle_degrees);
	if (sphere->bump.is_active)
		set_texture_rotation_degrees(&sphere->bump, angle_degrees);
}
