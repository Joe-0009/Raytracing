#ifndef MINIRT_H
# define MINIRT_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <float.h>
# include <limits.h>
# include <math.h>
# include <stdlib.h>
# include <unistd.h>

/* Define boolean values */
# define TRUE 1
# define FALSE 0

# include "parser.h"
# include "scene.h"
# include "math_utils.h"
# include <mlx.h>

/* Error codes */
# define ERR_ARGS "Error: Invalid number of arguments\n"
# define ERR_FILE "Error: Could not open file\n"
# define ERR_FORMAT "Error: Invalid file format\n"
# define ERR_SCENE "Error: Invalid scene configuration\n"
# define ERR_MEMORY "Error: Memory allocation failed\n"

/* Image structure */
typedef struct s_image
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_image;

/* Main program variables structure */
typedef struct s_vars
{
	void	*mlx;
	void	*win;
	t_image	*img;
}			t_vars;

/* Function prototypes */
void		error_exit(char *message);

/* Error utility functions */
void		ft_print_error(const char *message);
void		ft_print_error_detail(const char *message, const char *detail);
void		ft_print_number(double num);
void		ft_print_vector(t_vec3 v);

#endif
