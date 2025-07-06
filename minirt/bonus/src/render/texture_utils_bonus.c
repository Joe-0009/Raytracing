#include "../includes/scene_bonus.h"
#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*
** Load a texture using MLX image loading (XPM or PNG)
*/
t_texture	load_texture(void *mlx, const char *filename)
{
	t_texture	texture;
	char		*data_addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			i, j;
	int			pixel;
	
	texture.has_texture = 0;
	texture.data = NULL;
	texture.mlx_img = NULL;
	texture.width = 0;
	texture.height = 0;
	texture.path = NULL;
	texture.has_bump_map = 0;
	texture.bump_data = NULL;
	texture.bump_mlx_img = NULL;
	texture.bump_width = 0;
	texture.bump_height = 0;
	texture.bump_path = NULL;
	
	// Determine file type and load accordingly
	if (strstr(filename, ".xpm"))
		texture.mlx_img = mlx_xpm_file_to_image(mlx, (char *)filename, 
			&texture.width, &texture.height);
	else
	{
		printf("Error: Unsupported image format. Only XPM files are supported.\n");
		return (texture);
	}
	
	if (!texture.mlx_img)
		return (texture);
	
	// Get image data
	data_addr = mlx_get_data_addr(texture.mlx_img, &bits_per_pixel, 
		&line_length, &endian);
	if (!data_addr)
	{
		mlx_destroy_image(mlx, texture.mlx_img);
		return (texture);
	}
	
	// Convert MLX image data to our RGB format
	texture.data = malloc(texture.width * texture.height * 3);
	if (!texture.data)
	{
		mlx_destroy_image(mlx, texture.mlx_img);
		return (texture);
	}
	
	for (i = 0; i < texture.height; i++)
	{
		for (j = 0; j < texture.width; j++)
		{
			pixel = *(int *)(data_addr + (i * line_length + j * (bits_per_pixel / 8)));
			texture.data[(i * texture.width + j) * 3] = (pixel >> 16) & 0xFF; // R
			texture.data[(i * texture.width + j) * 3 + 1] = (pixel >> 8) & 0xFF; // G
			texture.data[(i * texture.width + j) * 3 + 2] = pixel & 0xFF; // B
		}
	}
	
	texture.has_texture = 1;
	return (texture);
}

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
	int			x, y;
	int			index;
	t_color3	color;
	
	if (!texture->has_texture || !texture->data)
		return (vec3_create(1.0, 1.0, 1.0));
	
	// Clamp UV coordinates to [0, 1]
	uv.u = fmod(uv.u, 1.0);
	uv.v = fmod(uv.v, 1.0);
	if (uv.u < 0) uv.u += 1.0;
	if (uv.v < 0) uv.v += 1.0;
	
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
	double	phi, theta;
	
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
t_uv	sphere_uv_mapping_with_rotation(t_vec3 point, t_vec3 center, double radius, t_vec3 rotation)
{
	t_uv	uv;
	t_vec3	normalized;
	double	phi, theta;
	
	// Get point relative to sphere center and normalize
	normalized = vec3_sub(point, center);
	normalized = vec3_div(normalized, radius);
	
	// Apply rotation to the normalized point
	// Rotate around Y axis (longitude rotation)
	if (rotation.y != 0.0)
	{
		double cos_y = cos(rotation.y);
		double sin_y = sin(rotation.y);
		double temp_x = normalized.x * cos_y - normalized.z * sin_y;
		double temp_z = normalized.x * sin_y + normalized.z * cos_y;
		normalized.x = temp_x;
		normalized.z = temp_z;
	}
	
	// Rotate around X axis (latitude rotation)
	if (rotation.x != 0.0)
	{
		double cos_x = cos(rotation.x);
		double sin_x = sin(rotation.x);
		double temp_y = normalized.y * cos_x - normalized.z * sin_x;
		double temp_z = normalized.y * sin_x + normalized.z * cos_x;
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
	
	ocean_color = vec3_create(0.1, 0.3, 0.8);  // Blue ocean
	land_color = vec3_create(0.2, 0.6, 0.2);   // Green land
	
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
** Load all textures in the scene after MLX initialization
*/
void	load_scene_textures(void *mlx, t_scene *scene)
{
	int	i;
	
	if (!mlx || !scene)
		return;
	
	// Load textures for all objects
	for (i = 0; i < scene->num_objects; i++)
	{
		if (scene->objects[i].type == SPHERE)
		{
			t_sphere *sphere = &scene->objects[i].data.sphere;
			
			// Load regular texture
			if (sphere->texture.has_texture && sphere->texture.path)
			{
				t_texture loaded_texture = load_texture(mlx, sphere->texture.path);
				if (loaded_texture.has_texture)
				{
					sphere->texture.width = loaded_texture.width;
					sphere->texture.height = loaded_texture.height;
					sphere->texture.data = loaded_texture.data;
					sphere->texture.mlx_img = loaded_texture.mlx_img;
				}
				else
				{
					sphere->texture.has_texture = 0;
				}
			}
			
			// Load bump map
			if (sphere->texture.has_bump_map && sphere->texture.bump_path)
			{
				t_texture loaded_bump = load_bump_map(mlx, sphere->texture.bump_path);
				if (loaded_bump.has_texture)
				{
					sphere->texture.bump_width = loaded_bump.width;
					sphere->texture.bump_height = loaded_bump.height;
					sphere->texture.bump_data = loaded_bump.data;
					sphere->texture.bump_mlx_img = loaded_bump.mlx_img;
				}
				else
				{
					sphere->texture.has_bump_map = 0;
				}
			}
		}
		else if (scene->objects[i].type == PLANE)
		{
			t_plane *plane = &scene->objects[i].data.plane;
			
			// Load regular texture
			if (plane->texture.has_texture && plane->texture.path)
			{
				t_texture loaded_texture = load_texture(mlx, plane->texture.path);
				if (loaded_texture.has_texture)
				{
					plane->texture.width = loaded_texture.width;
					plane->texture.height = loaded_texture.height;
					plane->texture.data = loaded_texture.data;
					plane->texture.mlx_img = loaded_texture.mlx_img;
				}
				else
				{
					printf("Warning: Could not load texture '%s'\n", plane->texture.path);
					plane->texture.has_texture = 0;
				}
			}
			
			// Load bump map
			if (plane->texture.has_bump_map && plane->texture.bump_path)
			{
				t_texture loaded_bump = load_bump_map(mlx, plane->texture.bump_path);
				if (loaded_bump.has_texture)
				{
					plane->texture.bump_width = loaded_bump.width;
					plane->texture.bump_height = loaded_bump.height;
					plane->texture.bump_data = loaded_bump.data;
					plane->texture.bump_mlx_img = loaded_bump.mlx_img;
				}
				else
				{
					printf("Warning: Could not load bump map '%s'\n", plane->texture.bump_path);
					plane->texture.has_bump_map = 0;
				}
			}
		}
		else if (scene->objects[i].type == CYLINDER)
		{
			t_cylinder *cylinder = &scene->objects[i].data.cylinder;
			
			// Load regular texture
			if (cylinder->texture.has_texture && cylinder->texture.path)
			{
				t_texture loaded_texture = load_texture(mlx, cylinder->texture.path);
				if (loaded_texture.has_texture)
				{
					cylinder->texture.width = loaded_texture.width;
					cylinder->texture.height = loaded_texture.height;
					cylinder->texture.data = loaded_texture.data;
					cylinder->texture.mlx_img = loaded_texture.mlx_img;
				}
				else
				{
					printf("Warning: Could not load texture '%s'\n", cylinder->texture.path);
					cylinder->texture.has_texture = 0;
				}
			}
			
			// Load bump map
			if (cylinder->texture.has_bump_map && cylinder->texture.bump_path)
			{
				t_texture loaded_bump = load_bump_map(mlx, cylinder->texture.bump_path);
				if (loaded_bump.has_texture)
				{
					cylinder->texture.bump_width = loaded_bump.width;
					cylinder->texture.bump_height = loaded_bump.height;
					cylinder->texture.bump_data = loaded_bump.data;
					cylinder->texture.bump_mlx_img = loaded_bump.mlx_img;
				}
				else
				{
					printf("Warning: Could not load bump map '%s'\n", cylinder->texture.bump_path);
					cylinder->texture.has_bump_map = 0;
				}
			}
		}
		else if (scene->objects[i].type == CONE)
		{
			t_cone *cone = &scene->objects[i].data.cone;
			
			// Load regular texture
			if (cone->texture.has_texture && cone->texture.path)
			{
				t_texture loaded_texture = load_texture(mlx, cone->texture.path);
				if (loaded_texture.has_texture)
				{
					cone->texture.width = loaded_texture.width;
					cone->texture.height = loaded_texture.height;
					cone->texture.data = loaded_texture.data;
					cone->texture.mlx_img = loaded_texture.mlx_img;
				}
				else
				{
					printf("Warning: Could not load texture '%s'\n", cone->texture.path);
					cone->texture.has_texture = 0;
				}
			}
			
			// Load bump map
			if (cone->texture.has_bump_map && cone->texture.bump_path)
			{
				t_texture loaded_bump = load_bump_map(mlx, cone->texture.bump_path);
				if (loaded_bump.has_texture)
				{
					cone->texture.bump_width = loaded_bump.width;
					cone->texture.bump_height = loaded_bump.height;
					cone->texture.bump_data = loaded_bump.data;
					cone->texture.bump_mlx_img = loaded_bump.mlx_img;
				}
				else
				{
					printf("Warning: Could not load bump map '%s'\n", cone->texture.bump_path);
					cone->texture.has_bump_map = 0;
				}
			}
		}
	}
}

/*
** Load a bump map texture (same as regular texture but for bump mapping)
*/
t_texture	load_bump_map(void *mlx, const char *filename)
{
	t_texture	texture;
	char		*data_addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			i, j;
	int			pixel;
	
	texture.has_texture = 0;
	texture.data = NULL;
	texture.mlx_img = NULL;
	texture.width = 0;
	texture.height = 0;
	texture.path = NULL;
	texture.has_bump_map = 0;
	texture.bump_data = NULL;
	texture.bump_mlx_img = NULL;
	texture.bump_width = 0;
	texture.bump_height = 0;
	texture.bump_path = NULL;
	
	// Determine file type and load accordingly
	if (strstr(filename, ".xpm"))
		texture.mlx_img = mlx_xpm_file_to_image(mlx, (char *)filename, 
			&texture.width, &texture.height);
	else
	{
		printf("Error: Unsupported bump map format. Only XPM files are supported.\n");
		return (texture);
	}
	
	if (!texture.mlx_img)
		return (texture);
	
	// Get image data
	data_addr = mlx_get_data_addr(texture.mlx_img, &bits_per_pixel, 
		&line_length, &endian);
	if (!data_addr)
	{
		mlx_destroy_image(mlx, texture.mlx_img);
		return (texture);
	}
	
	// Convert MLX image data to our RGB format (we'll use grayscale for bump)
	texture.data = malloc(texture.width * texture.height * 3);
	if (!texture.data)
	{
		mlx_destroy_image(mlx, texture.mlx_img);
		return (texture);
	}
	
	for (i = 0; i < texture.height; i++)
	{
		for (j = 0; j < texture.width; j++)
		{
			pixel = *(int *)(data_addr + (i * line_length + j * (bits_per_pixel / 8)));
			texture.data[(i * texture.width + j) * 3] = (pixel >> 16) & 0xFF; // R
			texture.data[(i * texture.width + j) * 3 + 1] = (pixel >> 8) & 0xFF; // G
			texture.data[(i * texture.width + j) * 3 + 2] = pixel & 0xFF; // B
		}
	}
	
	texture.has_texture = 1;
	return (texture);
}

/*
** Sample bump map at UV coordinates
** Returns height value (0.0 to 1.0) from grayscale bump map
*/
double	sample_bump_map(const t_texture *texture, t_uv uv)
{
	int		x, y;
	int		index;
	double	height;
	
	if (!texture->has_bump_map || !texture->bump_data || 
		texture->bump_width <= 0 || texture->bump_height <= 0)
		return (0.5);  // Return neutral height if no bump map
	
	// Clamp UV coordinates to [0, 1]
	uv.u = fmod(uv.u, 1.0);
	uv.v = fmod(uv.v, 1.0);
	if (uv.u < 0) uv.u += 1.0;
	if (uv.v < 0) uv.v += 1.0;
	
	// Convert UV to pixel coordinates
	x = (int)(uv.u * (texture->bump_width - 1));
	y = (int)(uv.v * (texture->bump_height - 1));
	
	// Bounds checking
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x >= texture->bump_width) x = texture->bump_width - 1;
	if (y >= texture->bump_height) y = texture->bump_height - 1;
	
	// Get height from bump map (use R channel as grayscale)
	index = (y * texture->bump_width + x) * 3;
	
	// Additional bounds checking for the data array
	if (index + 2 >= texture->bump_width * texture->bump_height * 3)
		return (0.5);
	
	height = texture->bump_data[index] / 255.0;
	
	// Enhance contrast of the bump map
	height = (height - 0.5) * 2.0 + 0.5;  // Increase contrast
	if (height < 0.0) height = 0.0;
	if (height > 1.0) height = 1.0;
	
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
	double	h_center, h_right, h_up;
	t_uv	uv_right, uv_up;
	t_vec3	bump_normal;
	double	du, dv;
	
	if (!texture->has_bump_map)
		return (normal);
	
	bump_scale = 2.0;  // Much higher bump scale for pronounced effect
	
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
	t_vec3 deviation = vec3_sub(bump_normal, normal);
	deviation = vec3_mult(deviation, 1.5);  // Amplify the deviation
	bump_normal = vec3_add(normal, deviation);
	
	return (vec3_normalize(bump_normal));
}
