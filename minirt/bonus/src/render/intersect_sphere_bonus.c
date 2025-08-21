/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_sphere_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:19:01 by isallali          #+#    #+#             */
/*   Updated: 2025/08/21 21:19:02 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt_app_bonus.h"
#include "../includes/scene_bonus.h"
#include <math.h>

/*
** Sample a color from a texture at given UV
*/
t_color3	sample_texture(const t_surface_map *texture, t_uv uv)
{
	int			idx;
	int			x;
	int			y;
	t_color3	color;

	color.x = 0;
	color.y = 0;
	color.z = 0;
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
double	sample_bump_map(const t_surface_map *bump, t_uv uv)
{
	int		idx;
	double	value;
	int		x;
	int		y;

	if (!bump->is_active || !bump->data)
		return (0.0);
	uv.u = fmod(uv.u + bump->rotation_uv.u, 1.0);
	x = (int)(uv.u * (bump->width - 1));
	y = (int)(uv.v * (bump->height - 1));
	idx = (y * bump->width + x) * 3;
	value = bump->data[idx];
	return (value / 255.0);
}

t_vec3	apply_sphere_bump(const t_sphere *sphere, t_hit *hit)
{
	t_uv	uv_right;
	t_uv	uv_up;
	t_vec3	tangent;
	t_vec3	bitangent;
	t_vec3	bumped;

	double (h_center), (h_right), (h_up), (du), (dv);
	if (!sphere->bump.is_active || !sphere->bump.data)
		return (hit->normal);
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
	bumped = vec3_add(hit->normal, vec3_add(vec3_mult(tangent, du),
				vec3_mult(bitangent, dv)));
	return (vec3_normalize(bumped));
}

t_vec3	determine_sphere_color(const t_sphere *sphere, t_hit *hit)
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
