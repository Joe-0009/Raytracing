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
** Free texture memory
*/
void	free_texture(void *mlx, t_texture *texture)
{
	if (texture->texture_data)
	{
		free(texture->texture_data);
		texture->texture_data = NULL;
	}
	if (texture->texture_mlx_img)
	{
		mlx_destroy_image(mlx, texture->texture_mlx_img);
		texture->texture_mlx_img = NULL;
	}
	if (texture->texture_path)
	{
		free(texture->texture_path);
		texture->texture_path = NULL;
	}
	texture->has_texture = 0;
}

/*
** Free bump map memory
*/
void	free_bump(void *mlx, t_bump *bump)
{
	if (bump->bump_data)
	{
		free(bump->bump_data);
		bump->bump_data = NULL;
	}
	if (bump->bump_mlx_img)
	{
		mlx_destroy_image(mlx, bump->bump_mlx_img);
		bump->bump_mlx_img = NULL;
	}
	if (bump->bump_path)
	{
		free(bump->bump_path);
		bump->bump_path = NULL;
	}
	bump->has_bump_map = 0;
}

/*
** Sample bump map at UV coordinates
** Returns height value (0.0 to 1.0) from grayscale bump map
*/
double	sample_bump_map(const t_bump *bump, t_uv uv)
{
	int		index;
	double	height;

	int x, y;
	if (!bump->has_bump_map || !bump->bump_data
		|| bump->bump_width <= 0 || bump->bump_height <= 0)
		return (0.5);
	uv.u = fmod(uv.u, 1.0);
	uv.v = fmod(uv.v, 1.0);
	if (uv.u < 0)
		uv.u += 1.0;
	if (uv.v < 0)
		uv.v += 1.0;
	x = (int)(uv.u * (bump->bump_width - 1));
	y = (int)(uv.v * (bump->bump_height - 1));
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x >= bump->bump_width)
		x = bump->bump_width - 1;
	if (y >= bump->bump_height)
		y = bump->bump_height - 1;
	index = (y * bump->bump_width + x) * 3;
	if (index + 2 >= bump->bump_width * bump->bump_height * 3)
		return (0.5);
	height = bump->bump_data[index] / 255.0;
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
t_color3	sample_texture(const t_texture *texture, t_uv uv)
{
	int			index;
	t_color3	color;

	int x, y;
	if (!texture->has_texture || !texture->texture_data)
		return (vec3_create(1.0, 1.0, 1.0));
	uv.u = fmod(uv.u, 1.0);
	uv.v = fmod(uv.v, 1.0);
	if (uv.u < 0)
		uv.u += 1.0;
	if (uv.v < 0)
		uv.v += 1.0;
	x = (int)(uv.u * (texture->texture_width - 1));
	y = (int)(uv.v * (texture->texture_height - 1));
	index = (y * texture->texture_width + x) * 3;
	color.x = texture->texture_data[index] / 255.0;
	color.y = texture->texture_data[index + 1] / 255.0;
	color.z = texture->texture_data[index + 2] / 255.0;
	return (color);
}


/*
** Apply bump mapping to surface normal
** Uses the bump map to perturb the surface normal for realistic lighting
*/
t_vec3	apply_bump_mapping(t_vec3 normal, t_uv uv, const t_bump *bump,
		t_vec3 tangent, t_vec3 bitangent)
{
	double	bump_scale;
	t_vec3	bump_normal;
	t_vec3	deviation;

	double h_center, h_right, h_up;
	t_uv uv_right, uv_up;
	double du, dv;
	if (!bump->has_bump_map)
		return (normal);
	bump_scale = 2.0; // Much higher bump scale for pronounced effect
	// Sample bump map at current position and neighboring positions
	h_center = sample_bump_map(bump, uv);
	uv_right = uv;
	uv_right.u += 1.0 / bump->bump_width;
	h_right = sample_bump_map(bump, uv_right);
	uv_up = uv;
	uv_up.v += 1.0 / bump->bump_height;
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
** Load a texture using MLX image loading (XPM or PNG)
*/
void	load_texture(void *mlx, t_texture *texture)
{
	char		*data_addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			pixel;
	int i, j;

	if (strstr(texture->texture_path, ".xpm"))
		texture->texture_mlx_img = mlx_xpm_file_to_image(mlx, (char *)texture->texture_path,
				&texture->texture_width, &texture->texture_height);
	else
	{
		printf("Error: Only XPM files are supported.\n");
		texture->has_texture = 0;
		return;
	}
	if (!texture->texture_mlx_img)
	{
		texture->has_texture = 0;
		return;
	}
	data_addr = mlx_get_data_addr(texture->texture_mlx_img, &bits_per_pixel,
			&line_length, &endian);
	if (!data_addr)
	{
		mlx_destroy_image(mlx, texture->texture_mlx_img);
		texture->has_texture = 0;
		return;
	}
	texture->texture_data = malloc(texture->texture_width * texture->texture_height * 3);
	if (!texture->texture_data)
	{
		mlx_destroy_image(mlx, texture->texture_mlx_img);
		texture->has_texture = 0;
		return;
	}
	i = -1;
	while (++i < texture->texture_height)
	{
		j = -1; 
		while (++j < texture->texture_width)
		{
			pixel = *(int *)(data_addr + (i * line_length + j * (bits_per_pixel
							/ 8)));
			texture->texture_data[(i * texture->texture_width + j) * 3] = (pixel >> 16) & 0xFF;
			texture->texture_data[(i * texture->texture_width + j) * 3 + 1] = (pixel >> 8) & 0xFF;
			texture->texture_data[(i * texture->texture_width + j) * 3 + 2] = pixel & 0xFF;
		}
	}
	texture->has_texture = 1;
}

/*
** Load a bump map texture (same as regular texture but for bump mapping)
*/
void	load_bump_map(void *mlx, t_bump *bump)
{
	char		*data_addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			pixel;
	int i, j;

	if (strstr(bump->bump_path, ".xpm"))
		bump->bump_mlx_img = mlx_xpm_file_to_image(mlx, (char *)bump->bump_path,
				&bump->bump_width, &bump->bump_height);
	else
	{
		printf("Error: Only XPM files are supported.\n");
		bump->has_bump_map = 0;
		return;
	}
	if (!bump->bump_mlx_img)
	{
		bump->has_bump_map = 0;
		return;
	}
	data_addr = mlx_get_data_addr(bump->bump_mlx_img, &bits_per_pixel,
			&line_length, &endian);
	if (!data_addr)
	{
		mlx_destroy_image(mlx, bump->bump_mlx_img);
		bump->has_bump_map = 0;
		return ;
	}
	bump->bump_data = malloc(bump->bump_width * bump->bump_height * 3);
	if (!bump->bump_data)
	{
		mlx_destroy_image(mlx, bump->bump_mlx_img);
		bump->has_bump_map = 0;
		return ;
	}
	i = -1;
	while (++i < bump->bump_height)
	{
		j = -1; 
		while (++j < bump->bump_width)
		{
			pixel = *(int *)(data_addr + (i * line_length + j * (bits_per_pixel
							/ 8)));
			bump->bump_data[(i * bump->bump_width + j) * 3] = (pixel >> 16) & 0xFF;
			bump->bump_data[(i * bump->bump_width + j) * 3 + 1] = (pixel >> 8) & 0xFF;
			bump->bump_data[(i * bump->bump_width + j) * 3 + 2] = pixel & 0xFF;
		}
	}
	bump->has_bump_map = 1;
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
			if (scene->objects[i].data.sphere.texture.has_texture && scene->objects[i].data.sphere.texture.texture_path)
				load_texture(mlx, &scene->objects[i].data.sphere.texture);
			if (scene->objects[i].data.sphere.bump.has_bump_map && scene->objects[i].data.sphere.bump.bump_path)
				load_bump_map(mlx, &scene->objects[i].data.sphere.bump);
		}
	}
}



