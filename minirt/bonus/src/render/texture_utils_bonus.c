#include "../includes/scene_bonus.h"
#include <math.h>
#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif



/*
** Free surface map memory (unified for texture and bump)
*/
void	free_surface_map(void *mlx, t_surface_map *surface_map)
{
	if (surface_map->data)
	{
		free(surface_map->data);
		surface_map->data = NULL;
	}
	if (surface_map->mlx_img)
	{
		mlx_destroy_image(mlx, surface_map->mlx_img);
		surface_map->mlx_img = NULL;
	}
	if (surface_map->path)
	{
		free(surface_map->path);
		surface_map->path = NULL;
	}
	surface_map->is_active = 0;
}

/*
** Sample bump map at UV coordinates
** Returns height value (0.0 to 1.0) from grayscale bump map
*/
double	sample_bump_map(const t_surface_map *bump, t_uv uv)
{
	int		index;
	double	height;

	int x, y;
	if (!bump->is_active || !bump->data
		|| bump->width <= 0 || bump->height <= 0)
		return (0.5);
	
	// Apply rotation if any rotation is set
	if (bump->rotation_x != 0.0 || bump->rotation_y != 0.0 || bump->rotation_z != 0.0)
		uv = apply_uv_rotation(uv, bump);
	
	uv.u = fmod(uv.u, 1.0);
	uv.v = fmod(uv.v, 1.0);
	if (uv.u < 0)
		uv.u += 1.0;
	if (uv.v < 0)
		uv.v += 1.0;
	x = (int)(uv.u * (bump->width - 1));
	y = (int)(uv.v * (bump->height - 1));
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x >= bump->width)
		x = bump->width - 1;
	if (y >= bump->height)
		y = bump->height - 1;
	index = (y * bump->width + x) * 3;
	if (index + 2 >= bump->width * bump->height * 3)
		return (0.5);
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
*/
t_color3	sample_texture(const t_surface_map *texture, t_uv uv)
{
	int			index;
	t_color3	color;

	int x, y;
	if (!texture->is_active || !texture->data)
		return (vec3_create(1.0, 1.0, 1.0));
	
	// Apply rotation if any rotation is set
	if (texture->rotation_x != 0.0 || texture->rotation_y != 0.0 || texture->rotation_z != 0.0)
		uv = apply_uv_rotation(uv, texture);
	
	uv.u = fmod(uv.u, 1.0);
	uv.v = fmod(uv.v, 1.0);
	if (uv.u < 0)
		uv.u += 1.0;
	if (uv.v < 0)
		uv.v += 1.0;
	x = (int)(uv.u * (texture->width - 1));
	y = (int)(uv.v * (texture->height - 1));
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
	t_uv uv_right, uv_up;
	double du, dv;
	if (!bump->is_active)
		return (normal);
	bump_scale = 2.0; // Much higher bump scale for pronounced effect
	// Sample bump map at current position and neighboring positions
	h_center = sample_bump_map(bump, uv);
	uv_right = uv;
	uv_right.u += 1.0 / bump->width;
	h_right = sample_bump_map(bump, uv_right);
	uv_up = uv;
	uv_up.v += 1.0 / bump->height;
	h_up = sample_bump_map(bump, uv_up);
	// Calculate gradients with much higher sensitivity
	du = (h_right - h_center) * bump_scale * 5.0;
	dv = (h_up - h_center) * bump_scale * 5.0;
	// Enhance the bump effect further
	du = du * (1.0 + fabs(h_center - 0.5) * 2.0);
	dv = dv * (1.0 + fabs(h_center - 0.5) * 2.0);
	// Perturb normal using tangent space with stronger effect
	bump_normal = vec3_add(normal, vec3_mult(tangent, du));
	bump_normal = vec3_add(bump_normal, vec3_mult(bitangent, dv));
	// Enhance the deviation from the original normal
	deviation = vec3_sub(bump_normal, normal);
	deviation = vec3_mult(deviation, 1.5); // Amplify the deviation
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
** Load a surface map using MLX image loading (XPM or PNG)
** Works for both textures and bump maps based on map_type
*/
void	load_surface_map(void *mlx, t_surface_map *surface_map)
{
	char		*data_addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			pixel;
	int i, j;

	if (strstr(surface_map->path, ".xpm"))
		surface_map->mlx_img = mlx_xpm_file_to_image(mlx, (char *)surface_map->path,
				&surface_map->width, &surface_map->height);
	else
	{
		printf("Error: Only XPM files are supported.\n");
		surface_map->is_active = 0;
		return;
	}
	if (!surface_map->mlx_img)
	{
		surface_map->is_active = 0;
		return;
	}
	data_addr = mlx_get_data_addr(surface_map->mlx_img, &bits_per_pixel,
			&line_length, &endian);
	if (!data_addr)
	{
		mlx_destroy_image(mlx, surface_map->mlx_img);
		surface_map->is_active = 0;
		return;
	}
	surface_map->data = malloc(surface_map->width * surface_map->height * 3);
	if (!surface_map->data)
	{
		mlx_destroy_image(mlx, surface_map->mlx_img);
		surface_map->is_active = 0;
		return;
	}
	i = -1;
	while (++i < surface_map->height)
	{
		j = -1; 
		while (++j < surface_map->width)
		{
			pixel = *(int *)(data_addr + (i * line_length + j * (bits_per_pixel
							/ 8)));
			surface_map->data[(i * surface_map->width + j) * 3] = (pixel >> 16) & 0xFF;
			surface_map->data[(i * surface_map->width + j) * 3 + 1] = (pixel >> 8) & 0xFF;
			surface_map->data[(i * surface_map->width + j) * 3 + 2] = pixel & 0xFF;
		}
	}
	surface_map->is_active = 1;
}

/*
** Load all textures in the scene after MLX initialization
*/
void	load_scene_texture_bump(void *mlx, t_scene *scene)
{
	int			i;

	if (!mlx || !scene)
		return ;
	i = -1;
	while (++i < scene->num_objects)
	{
		if (scene->objects[i].type == SPHERE)
		{
			if (scene->objects[i].data.sphere.texture.is_active && scene->objects[i].data.sphere.texture.path)
				load_surface_map(mlx, &scene->objects[i].data.sphere.texture);
			if (scene->objects[i].data.sphere.bump.is_active && scene->objects[i].data.sphere.bump.path)
				load_surface_map(mlx, &scene->objects[i].data.sphere.bump);
		}
	}
}

/*
** Apply rotation to UV coordinates
** Rotates UV coordinates around the center (0.5, 0.5) using the rotation angles
*/
t_uv	apply_uv_rotation(t_uv uv, const t_surface_map *surface_map)
{
	t_uv	rotated;
	double	cos_z, sin_z;
	double	temp_u, temp_v;

	// Only apply Z rotation for 2D UV space (most common case)
	// Center the UV coordinates around (0, 0)
	temp_u = uv.u - 0.5;
	temp_v = uv.v - 0.5;
	
	// Apply Z rotation (rotation in the UV plane)
	cos_z = cos(surface_map->rotation_z);
	sin_z = sin(surface_map->rotation_z);
	
	rotated.u = temp_u * cos_z - temp_v * sin_z;
	rotated.v = temp_u * sin_z + temp_v * cos_z;
	
	// Translate back to center around (0.5, 0.5)
	rotated.u += 0.5;
	rotated.v += 0.5;
	
	return (rotated);
}



