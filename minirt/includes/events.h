/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 10:00:00 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/22 20:31:52 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTS_H
# define EVENTS_H

# include "minirt.h"

/* Key codes */
# define KEY_ESC 65307

/* Function prototypes for events */
int		close_window_x(t_vars *vars);
int		close_window_esc(int keycode, t_vars *vars);
int		key_handler(int keycode, t_vars *vars);
void	mlx_hooks(t_vars *vars);

#endif
