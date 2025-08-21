/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_handlers_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:20:52 by isallali          #+#    #+#             */
/*   Updated: 2025/08/21 21:20:53 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/events_bonus.h"
#include "../../includes/minirt_app_bonus.h"
#include "../../includes/scene_bonus.h"
#include <stdio.h>

static void	handle_sphere_texture_rotation(t_scene *scene, int keycode)
{
	t_sphere	*sphere;

	sphere = &scene->objects[scene->selected_obj].data.sphere;
	if (keycode == KEY_R)
	{
		if (sphere->texture.is_active)
			sphere->texture.rotation_uv.u += 0.05;
		if (sphere->bump.is_active)
			sphere->bump.rotation_uv.u += 0.05;
	}
	else if (keycode == KEY_F)
	{
		if (sphere->texture.is_active)
			sphere->texture.rotation_uv.u += -0.05;
		if (sphere->bump.is_active)
			sphere->bump.rotation_uv.u += -0.05;
	}
}

static void	handle_object_scale_rotation(int keycode, t_scene *scene)
{
	if (keycode == KEY_PLUS)
		scene_scale_object(scene, scene->selected_obj, 1.1);
	else if (keycode == KEY_MINUS)
		scene_scale_object(scene, scene->selected_obj, 0.9);
	else if (keycode == KEY_R || keycode == KEY_F || keycode == KEY_T
		|| keycode == KEY_G)
	{
		if (scene->objects[scene->selected_obj].type == SPHERE)
			handle_sphere_texture_rotation(scene, keycode);
		else
		{
			if (keycode == KEY_R)
				scene_rotate_object(scene, scene->selected_obj, vec3_create(0.2,
						0, 0));
			else if (keycode == KEY_T)
				scene_rotate_object(scene, scene->selected_obj, vec3_create(0,
						0.2, 0));
			else if (keycode == KEY_F)
				scene_rotate_object(scene, scene->selected_obj,
					vec3_create(-0.2, 0, 0));
			else if (keycode == KEY_G)
				scene_rotate_object(scene, scene->selected_obj, vec3_create(0,
						-0.2, 0));
		}
	}
}

static void	handle_object_translation(int keycode, t_scene *scene)
{
	if (keycode == KEY_P && scene->selected_obj < scene->num_objects - 1)
		scene->selected_obj++;
	else if (keycode == KEY_O && scene->selected_obj > 0)
		scene->selected_obj--;
	else if (keycode == KEY_LEFT)
		scene_translate_object(scene, scene->selected_obj, vec3_create(-0.3, 0,
				0));
	else if (keycode == KEY_RIGHT)
		scene_translate_object(scene, scene->selected_obj, vec3_create(0.3, 0,
				0));
	else if (keycode == KEY_UP)
		scene_translate_object(scene, scene->selected_obj, vec3_create(0, 0.3,
				0));
	else if (keycode == KEY_DOWN)
		scene_translate_object(scene, scene->selected_obj, vec3_create(0, -0.3,
				0));
}

void	handle_object_transforms(int keycode, t_scene *scene)
{
	handle_object_scale_rotation(keycode, scene);
	handle_object_translation(keycode, scene);
}
