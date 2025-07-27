#ifndef MINIRT_APP_BONUS_H
# define MINIRT_APP_BONUS_H

# include "../../libft/libft.h"
# include <fcntl.h>
# include <float.h>
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

# include "constants_bonus.h"
# include "parser_bonus.h"
# include "scene_bonus.h"

/* Image structure */
typedef struct s_image
{
	void				*img;
	char				*addr;
	int					bits_per_pixel;
	int					line_length;
	int					endian;
}						t_image;

/* Main program variables structure */
typedef struct s_vars
{
	void				*mlx;
	void				*win;
	
	t_image				*img;
	t_scene				*scene;
}						t_vars;

typedef struct s_hit	t_hit;

/* Thread data structure for parallel rendering */
typedef struct s_thread_data
{
	t_vars				*vars;
	t_scene				*scene;
	int					start_y;
	int					end_y;
	int					thread_id;
}						t_thread_data;

/* Function prototypes */
void					draw_new_image(t_vars *vars, t_scene *scene);
void					create_image(t_vars *vars);
void					main_draw(t_vars *vars, t_scene *scene);
void					*render_thread(void *arg);
void					put_pixel(t_vars *vars, int x, int y, int color);
void					error_exit(char *message);
int						close_window_esc(int keycode, t_vars *vars);
int						close_window_x(t_vars *vars);

#endif
