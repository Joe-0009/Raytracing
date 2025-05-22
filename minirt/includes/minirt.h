#ifndef MINIRT_H
# define MINIRT_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include <limits.h>
# include <float.h>
# include "../libft/libft.h"

/* Define boolean values */
# define TRUE 1
# define FALSE 0

# include <mlx.h>
# include "vector.h"
# include "scene.h"
# include "parser.h"
# include "main.h"
# include "events.h"

/* Error codes */
# define ERR_ARGS "Error: Invalid number of arguments\n"
# define ERR_FILE "Error: Could not open file\n"
# define ERR_FORMAT "Error: Invalid file format\n"
# define ERR_SCENE "Error: Invalid scene configuration\n"
# define ERR_MEMORY "Error: Memory allocation failed\n"

/* Function prototypes */
void    error_exit(char *message);



/* Error utility functions */
void    ft_print_error(const char *message);
void    ft_print_error_detail(const char *message, const char *detail);
void    ft_print_number(double num);
void    ft_print_vector(t_vec3 v);

#endif
