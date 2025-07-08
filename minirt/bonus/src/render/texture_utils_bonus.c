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
	if (texture->data)
	{
		free(texture->data);
		texture->data = NULL;
	}
	if (texture->mlx_img)
	{
		mlx_destroy_image(mlx, texture->mlx_img);
		texture->mlx_img = NULL;
	}
	if (texture->bump_data)
	{
		free(texture->bump_data);
		texture->bump_data = NULL;
	}
	if (texture->bump_mlx_img)
	{
		mlx_destroy_image(mlx, texture->bump_mlx_img);
		texture->bump_mlx_img = NULL;
	}
	if (texture->path)
	{
		free(texture->path);
		texture->path = NULL;
	}
	if (texture->bump_path)
	{
		free(texture->bump_path);
		texture->bump_path = NULL;
	}
	texture->has_texture = 0;
	texture->has_bump_map = 0;
}

/*
** Sample texture at UV coordinates
*/
t_color3	sample_texture(const t_texture *texture, t_uv uv)
{
	int			index;
	t_color3	color;

	int x, y;
	if (!texture->has_texture || !texture->data)
		return (vec3_create(1.0, 1.0, 1.0));
	// Clamp UV coordinates to [0, 1]
	uv.u = fmod(uv.u, 1.0);
	uv.v = fmod(uv.v, 1.0);
	if (uv.u < 0)
		uv.u += 1.0;
	if (uv.v < 0)
		uv.v += 1.0;
	// Convert UV to pixel coordinates
	x = (int)(uv.u * (texture->width - 1));
	y = (int)(uv.v * (texture->height - 1));
	// Get pixel color
	index = (y * texture->width + x) * 3;
	color.x = texture->data[index] / 255.0;
	color.y = texture->data[index + 1] / 255.0;
	color.z = texture->data[index + 2] / 255.0;
	return (color);
}

/*
** Calculate UV coordinates for sphere mapping
** Uses spherical coordinates for Earth-like mapping
*/
t_uv	sphere_uv_mapping(t_vec3 point, t_vec3 center, double radius)
{
	t_uv	uv;
	t_vec3	normalized;

	double phi, theta;
	// Get point relative to sphere center and normalize
	normalized = vec3_sub(point, center);
	normalized = vec3_div(normalized, radius);
	// Calculate spherical coordinates
	phi = atan2(normalized.z, normalized.x);
	theta = acos(normalized.y);
	// Convert to UV coordinates
	uv.u = (phi + M_PI) / (2.0 * M_PI);
	uv.v = theta / M_PI;
	return (uv);
}

/*
** UV mapping for sphere with rotation support
** Uses spherical coordinates for Earth-like mapping with rotation
*/
t_uv	sphere_uv_mapping_with_rotation(t_vec3 point, t_vec3 center,
		double radius, t_vec3 rotation)
{
	t_uv	uv;
	t_vec3	normalized;
	double	cos_y;
	double	sin_y;
	double	temp_x;
	double	temp_z;
	double	cos_x;
	double	sin_x;
	double	temp_y;
	double	temp_z;

	double phi, theta;
	// Get point relative to sphere center and normalize
	normalized = vec3_sub(point, center);
	normalized = vec3_div(normalized, radius);
	// Apply rotation to the normalized point
	// Rotate around Y axis (longitude rotation)
	if (rotation.y != 0.0)
	{
		cos_y = cos(rotation.y);
		sin_y = sin(rotation.y);
		temp_x = normalized.x * cos_y - normalized.z * sin_y;
		temp_z = normalized.x * sin_y + normalized.z * cos_y;
		normalized.x = temp_x;
		normalized.z = temp_z;
	}
	// Rotate around X axis (latitude rotation)
	if (rotation.x != 0.0)
	{
		cos_x = cos(rotation.x);
		sin_x = sin(rotation.x);
		temp_y = normalized.y * cos_x - normalized.z * sin_x;
		temp_z = normalized.y * sin_x + normalized.z * cos_x;
		normalized.y = temp_y;
		normalized.z = temp_z;
	}
	// Calculate spherical coordinates
	phi = atan2(normalized.z, normalized.x);
	theta = acos(normalized.y);
	// Convert to UV coordinates
	uv.u = (phi + M_PI) / (2.0 * M_PI);
	uv.v = theta / M_PI;
	return (uv);
}

/*
** Create a procedural Earth-like texture
** This creates a simple Earth pattern without loading an image
*/
t_color3	procedural_earth_texture(t_uv uv)
{
	t_color3	ocean_color;
	t_color3	land_color;
	t_color3	result;
	double		noise;
	double		land_mask;

	ocean_color = vec3_create(0.1, 0.3, 0.8); // Blue ocean
	land_color = vec3_create(0.2, 0.6, 0.2);  // Green land
	// Simple noise function for continents
	noise = sin(uv.u * 20.0) * cos(uv.v * 15.0);
	noise += sin(uv.u * 40.0) * cos(uv.v * 30.0) * 0.5;
	noise += sin(uv.u * 80.0) * cos(uv.v * 60.0) * 0.25;
	// Create land mask
	land_mask = (noise > 0.2) ? 1.0 : 0.0;
	// Mix ocean and land colors
	result = vec3_mult(ocean_color, 1.0 - land_mask);
	result = vec3_add(result, vec3_mult(land_color, land_mask));
	// Add some variation to make it more interesting
	if (land_mask > 0.5)
	{
		// Add brown/desert areas
		if (fabs(uv.v - 0.5) < 0.2 && sin(uv.u * 10.0) > 0.3)
			result = vec3_create(0.8, 0.6, 0.3);
		// Add white for ice caps
		if (uv.v < 0.1 || uv.v > 0.9)
			result = vec3_create(0.9, 0.9, 0.9);
	}
	return (result);
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
		return;
	}
	if (!texture->texture_mlx_img)
		return;
	data_addr = mlx_get_data_addr(texture->texture_mlx_img, &bits_per_pixel,
			&line_length, &endian);
	if (!data_addr)
	{
		mlx_destroy_image(mlx, texture->texture_mlx_img);
		return;
	}
	texture->texture_data = malloc(texture->texture_width * texture->texture_height * 3);
	if (!texture->texture_data)
	{
		mlx_destroy_image(mlx, texture->texture_mlx_img);
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
		return;
	}
	if (!bump->bump_mlx_img)
		return;
	data_addr = mlx_get_data_addr(bump->bump_mlx_img, &bits_per_pixel,
			&line_length, &endian);
	if (!data_addr)
	{
		mlx_destroy_image(mlx, bump->bump_mlx_img);
		return ;
	}
	bump->bump_data = malloc(bump->bump_width * bump->bump_height * 3);
	if (!bump->bump_data)
	{
		mlx_destroy_image(mlx, bump->bump_mlx_img);
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

/*
** Sample bump map at UV coordinates
** Returns height value (0.0 to 1.0) from grayscale bump map
*/
double	sample_bump_map(const t_texture *texture, t_uv uv)
{
	int		index;
	double	height;

	int x, y;
	if (!texture->has_bump_map || !texture->bump_data
		|| texture->bump_width <= 0 || texture->bump_height <= 0)
		return (0.5); // Return neutral height if no bump map
	// Clamp UV coordinates to [0, 1]
	uv.u = fmod(uv.u, 1.0);
	uv.v = fmod(uv.v, 1.0);
	if (uv.u < 0)
		uv.u += 1.0;
	if (uv.v < 0)
		uv.v += 1.0;
	// Convert UV to pixel coordinates
	x = (int)(uv.u * (texture->bump_width - 1));
	y = (int)(uv.v * (texture->bump_height - 1));
	// Bounds checking
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x >= texture->bump_width)
		x = texture->bump_width - 1;
	if (y >= texture->bump_height)
		y = texture->bump_height - 1;
	// Get height from bump map (use R channel as grayscale)
	index = (y * texture->bump_width + x) * 3;
	// Additional bounds checking for the data array
	if (index + 2 >= texture->bump_width * texture->bump_height * 3)
		return (0.5);
	height = texture->bump_data[index] / 255.0;
	// Enhance contrast of the bump map
	height = (height - 0.5) * 2.0 + 0.5; // Increase contrast
	if (height < 0.0)
		height = 0.0;
	if (height > 1.0)
		height = 1.0;
	return (height);
}

/*
** Apply bump mapping to surface normal
** Uses the bump map to perturb the surface normal for realistic lighting
*/
t_vec3	apply_bump_mapping(t_vec3 normal, t_uv uv, const t_texture *texture,
		t_vec3 tangent, t_vec3 bitangent)
{
	double	bump_scale;
	t_vec3	bump_normal;
	t_vec3	deviation;

	double h_center, h_right, h_up;
	t_uv uv_right, uv_up;
	double du, dv;
	if (!texture->has_bump_map)
		return (normal);
	bump_scale = 2.0; // Much higher bump scale for pronounced effect
	// Sample bump map at current position and neighboring positions
	h_center = sample_bump_map(texture, uv);
	// Sample neighboring pixels for gradient calculation
	uv_right = uv;
	uv_right.u += 1.0 / texture->bump_width;
	h_right = sample_bump_map(texture, uv_right);
	uv_up = uv;
	uv_up.v += 1.0 / texture->bump_height;
	h_up = sample_bump_map(texture, uv_up);
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
