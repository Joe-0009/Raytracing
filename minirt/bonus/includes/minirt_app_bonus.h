/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_app_bonus.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:34:57 by isallali          #+#    #+#             */
/*   Updated: 2025/08/21 21:49:47 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_APP_BONUS_H
# define MINIRT_APP_BONUS_H

# include "constants_bonus.h"
# include "libft.h"
# include "parser_bonus.h"
# include "scene_bonus.h"
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <mlx.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>

# define TRUE 1
# define FALSE 0
# define WIDTH 1200
# define HEIGHT 1000
# define WINDOW_NAME_RT "miniRT"
# define NUM_THREADS 8

typedef struct s_image
{
	void				*img;
	char				*addr;
	int					bits_per_pixel;
	int					line_length;
	int					endian;
}						t_image;

typedef struct s_vars
{
	void				*mlx;
	void				*win;

	t_image				*img;
	t_scene				*scene;
}						t_vars;

typedef struct s_hit	t_hit;

typedef struct s_thread_data
{
	t_vars				*vars;
	t_scene				*scene;
	int					start_y;
	int					end_y;
	int					thread_id;
}						t_thread_data;

void					draw_new_image(t_vars *vars, t_scene *scene);
void					create_image(t_vars *vars);
void					main_draw(t_vars *vars, t_scene *scene);
void					*render_thread(void *arg);
void					put_pixel(t_vars *vars, int x, int y, int color);
void					error_exit(char *message);
int						close_window_esc(int keycode, t_vars *vars);
int						close_window_x(t_vars *vars);

#endif