#ifndef MINIRT_APP_H
# define MINIRT_APP_H

# include "libft.h"
# include <fcntl.h>
# include <float.h>
# include <limits.h>
# include <math.h>
# include <mlx.h>
# include <stdlib.h>
# include <unistd.h>

# define TRUE 1
# define FALSE 0

# define WIDTH 1200
# define HEIGHT 1000
# define WINDOW_NAME_RT "miniRT"

# include "constants.h"
# include "parser.h"
# include "scene.h"

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

/* Function prototypes */
void					draw_new_image(t_vars *vars, t_scene *scene);
void					create_image(t_vars *vars);
void					main_draw(t_vars *vars, t_scene *scene);
void					put_pixel(t_vars *vars, int x, int y, int color);
void					error_exit(char *message);
void					print_scene_info(t_scene *scene);

#endif
