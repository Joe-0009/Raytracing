/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:21:00 by isallali          #+#    #+#             */
/*   Updated: 2025/08/21 21:21:01 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/events_bonus.h"
#include "../../includes/minirt_app_bonus.h"
#include "../../includes/scene_bonus.h"
#include <stdio.h>

void	draw_new_image(t_vars *vars, t_scene *scene)
{
	mlx_destroy_image(vars->mlx, vars->img->img);
	create_image(vars);
	main_draw(vars, scene);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img->img, 0, 0);
}

static int	is_redraw_key(int keycode)
{
	if (keycode == KEY_W || keycode == KEY_A || keycode == KEY_S
		|| keycode == KEY_D || keycode == KEY_Q || keycode == KEY_E
		|| keycode == KEY_I || keycode == KEY_J || keycode == KEY_K
		|| keycode == KEY_L || keycode == KEY_UP || keycode == KEY_DOWN
		|| keycode == KEY_LEFT || keycode == KEY_RIGHT || keycode == KEY_PLUS
		|| keycode == KEY_MINUS || keycode == KEY_R || keycode == KEY_T
		|| keycode == KEY_F || keycode == KEY_G || keycode == KEY_P
		|| keycode == KEY_O || keycode == KEY_U || keycode == KEY_Y
		|| keycode == KEY_H || keycode == KEY_N || keycode == KEY_V
		|| keycode == KEY_B || keycode == KEY_M || keycode == KEY_COMMA
		|| keycode == KEY_DOT)
		return (1);
	return (0);
}

int	key_handler(int keycode, t_vars *vars)
{
	if (keycode == 65307)
		close_window_esc(keycode, vars);
	else if (vars->scene)
	{
		printf("Keycode: %d\n", keycode);
		handle_camera_movement(keycode, vars->scene);
		handle_camera_rotation(keycode, vars->scene);
		handle_object_transforms(keycode, vars->scene);
		handle_light_movement(keycode, vars->scene);
		if (is_redraw_key(keycode))
			draw_new_image(vars, vars->scene);
	}
	if (keycode == 32)
		print_controls_help();
	return (0);
}

int	mouse_handler(int button, int x, int y, void *param)
{
	t_vars	*vars;
	t_ray	ray;
	t_hit	hit;

	vars = (t_vars *)param;
	if (button == 1)
	{
		ray = generate_camera_ray(vars->scene, x, y);
		if (trace_objects(vars->scene, ray, &hit))
			vars->scene->selected_obj = hit.obj_index;
		draw_new_image(vars, vars->scene);
	}
	return (0);
}

void	mlx_hooks(t_vars *vars)
{
	mlx_hook(vars->win, 4, 1L << 2, mouse_handler, vars);
	mlx_hook(vars->win, 2, 1L << 0, key_handler, vars);
	mlx_hook(vars->win, 17, 0, close_window_x, vars);
}
