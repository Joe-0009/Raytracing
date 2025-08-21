/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_load_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:19:15 by isallali          #+#    #+#             */
/*   Updated: 2025/08/21 21:19:16 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/scene_bonus.h"
#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int	load_image_file(void *mlx, t_surface_map *surface_map)
{
	if (strstr(surface_map->path, ".xpm"))
		surface_map->mlx_img = mlx_xpm_file_to_image(mlx,
				(char *)surface_map->path, &surface_map->width,
				&surface_map->height);
	else
	{
		printf("Error: Only XPM files are supported.\n");
		surface_map->is_active = 0;
		return (0);
	}
	if (!surface_map->mlx_img)
	{
		surface_map->is_active = 0;
		return (0);
	}
	return (1);
}

static void	convert_image_data(t_surface_map *surface_map, char *data_addr,
		int line_length, int bits_per_pixel)
{
	int	i;
	int	j;
	int	pixel;

	i = -1;
	while (++i < surface_map->height)
	{
		j = -1;
		while (++j < surface_map->width)
		{
			pixel = *(int *)(data_addr + (i * line_length + j * (bits_per_pixel
							/ 8)));
			surface_map->data[(i * surface_map->width + j)
				* 3] = (pixel >> 16) & 0xFF;
			surface_map->data[(i * surface_map->width + j) * 3
				+ 1] = (pixel >> 8) & 0xFF;
			surface_map->data[(i * surface_map->width + j) * 3
				+ 2] = pixel & 0xFF;
		}
	}
}

void	load_surface_map(void *mlx, t_surface_map *surface_map)
{
	char	*data_addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;

	if (!load_image_file(mlx, surface_map))
		return ;
	data_addr = mlx_get_data_addr(surface_map->mlx_img, &bits_per_pixel,
			&line_length, &endian);
	if (!data_addr)
	{
		mlx_destroy_image(mlx, surface_map->mlx_img);
		surface_map->is_active = 0;
		return ;
	}
	surface_map->data = malloc(surface_map->width * surface_map->height * 3);
	if (!surface_map->data)
	{
		mlx_destroy_image(mlx, surface_map->mlx_img);
		surface_map->is_active = 0;
		return ;
	}
	convert_image_data(surface_map, data_addr, line_length, bits_per_pixel);
	surface_map->is_active = 1;
}

void	load_scene_texture_bump(void *mlx, t_scene *scene)
{
	int	i;

	if (!mlx || !scene)
		return ;
	i = -1;
	while (++i < scene->num_objects)
	{
		if (scene->objects[i].type == SPHERE)
		{
			if (scene->objects[i].data.sphere.texture.is_active
				&& scene->objects[i].data.sphere.texture.path)
				load_surface_map(mlx, &scene->objects[i].data.sphere.texture);
			if (scene->objects[i].data.sphere.bump.is_active
				&& scene->objects[i].data.sphere.bump.path)
				load_surface_map(mlx, &scene->objects[i].data.sphere.bump);
		}
	}
}
