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
