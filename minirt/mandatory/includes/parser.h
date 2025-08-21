/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:33:45 by isallali          #+#    #+#             */
/*   Updated: 2025/08/21 21:43:56 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "scene.h"

typedef struct s_parser
{
	char	*line;
	char	**tokens;
	int		line_count;
	int		has_camera;
}			t_parser;

# define ERR_ARGS "Err: Invalid number of arguments\n"
# define ERR_FORMAT "Err: Invalid file format\n"
# define ERR_SCENE "Err: Invalid scene configuration\n"
# define ERR_MEMORY "Err: Memory allocation failed\n"
# define ERR_VALUE_RANGE "Err: Value out of allowed range\n"

# define ERR_FILE_EXTENSION "Err: File must have .rt extension\n"
# define ERR_FILE_ACCESS "Err: Could not open file %s\n"
# define ERR_UNKNOWN_ELEMENT "Err: Unknown element in scene file\n"
# define ERR_DUPLICATE_ELEMENT "Err: Duplicate unique element in scene file\n"
# define ERR_MISSING_ELEMENT "Err: Required element missing in scene file\n"
# define ERR_UNKNOWN_IDENTIFIER "Err: Line %d: Unknown identifier '%s'\n"
# define ERR_VECTOR_FORMAT "Err: Invalid vector format\n"
# define ERR_COLOR_FORMAT "Err: Invalid color format\n"

# define ERR_SCENE_NO_CAMERA "Err: Camera not defined\n"
# define ERR_SCENE_NO_AMBIENT "Err: Ambient lighting not defined\n"
# define ERR_SCENE_NO_LIGHT "Err: Light source not defined\n"
# define ERR_SCENE_NO_CAMERA_RENDER "Err: No camera in scene\n"

# define ERR_CAMERA_FORMAT "Err: Invalid camera format\n"
# define ERR_CAMERA_FOV_RANGE "Camera FOV must be in [0, 180] degrees\n"
# define ERR_CAMERA_ORIENTATION_ZERO "Err: Camera orientation zero\n"
# define FMT_CAMERA_EXPECTED "Expected format: C x,y,z nx,ny,nz fov\n"

# define ERR_AMBIENT_FORMAT "Err: Invalid ambient lighting format\n"
# define ERR_AMBIENT_ALREADY_DEFINED "Err: Ambient lighting already defined\n"
# define ERR_AMBIENT_RATIO_RANGE "Err: Ambient ratio must be in [0.0, 1.0]\n"
# define ERR_AMBIENT_COLOR_INVALID "Err: Invalid color for ambient lighting\n"
# define FMT_AMBIENT_EXPECTED "Expected format: A ratio r,g,b\n"
# define ERR_LIGHT_FORMAT "Err: Invalid light format\n"
# define ERR_LIGHT_ALREADY_DEFINED "Err: Light source already defined\n"
# define ERR_LIGHT_BRIGHTNESS_RANGE "Err: Light brightness in [0.0, 1.0]\n"
# define ERR_LIGHT_COLOR_INVALID "Err: Invalid color for light source\n"
# define FMT_LIGHT_EXPECTED "Expected format: L x,y,z brightness r,g,b\n"

# define ERR_SPHERE_FORMAT "Err: Invalid sphere format\n"
# define ERR_SPHERE_COLOR_INVALID "Err: Invalid color for sphere\n"
# define ERR_SPHERE_DIAMETER_POSITIVE "Sphere diameter must be positive\n"
# define WARN_SPHERE_DIAMETER_SMALL "Warning: Very small sphere diameter\n"
# define FMT_SPHERE_EXPECTED "Expected format: sp x,y,z diameter r,g,b\n"

# define ERR_PLANE_FORMAT "Err: Invalid plane format\n"
# define ERR_PLANE_COLOR_INVALID "Err: Invalid color for plane\n"
# define ERR_PLANE_NORMAL_ZERO "Err: Plane normal vector cannot be (0,0,0)\n"
# define ERR_PLANE_NORMAL_NOT_NORMALIZED "Err: Plane normal not normalized\n"
# define FMT_PLANE_EXPECTED "Expected format: pl x,y,z nx,ny,nz r,g,b\n"

# define ERR_CYLINDER_FORMAT "Err: Invalid cylinder format\n"
# define ERR_CYLINDER_COLOR_INVALID "Err: Invalid cylinder color\n"
# define ERR_CYLINDER_AXIS_ZERO "Err: Cylinder axis cannot be zero\n"
# define ERR_CYLINDER_AXIS_NOT_NORMALIZED "Err: Cylinder axis not normalized\n"
# define ERR_CYLINDER_DIMS_POSITIVE "Err: Cylinder dims must be positive\n"
# define ERR_CYLINDER_HEIGHT_NEGATIVE "Err: Cylinder height negative\n"
# define WARN_CYLINDER_DIMS_SMALL "Warning: Cylinder dimensions are small\n"
# define FMT_CYLINDER_EXPECTED "Expected: cy x,y,z nx,ny,nz D H r,g,b\n"

# define WARN_POSITION_FAR "Warning: %s far from origin (%.2f, %.2f, %.2f)\n"

/* Function prototypes */
/* File and scene loading */
t_scene		*parse_scene_file(char *filename);
int			validate_scene(t_scene *scene);
// int			validate_scene_rendering(t_scene *scene);

/* Element parsing functions */
int			parse_ambient(char **tokens, t_scene *scene);
int			parse_light(char **tokens, t_scene *scene);
int			parse_camera(char **tokens, t_scene *scene);
int			parse_sphere(char **tokens, t_scene *scene);
int			parse_plane(char **tokens, t_scene *scene);
int			parse_cylinder(char **tokens, t_scene *scene);

/* Data type parsing */
int			parse_vector(char *str, t_vec3 *vec);
int			parse_color(char *str, t_color3 *color);
int			parse_double(char *str, double *value);

/* Object validation */
int			validate_sphere(t_sphere *sphere);
int			validate_cylinder(t_cylinder *cylinder);
int			validate_plane(t_plane *plane);
int			validate_position(t_point3 pos, const char *type);
int			validate_non_zero_vector(t_vec3 vec);
int			validate_normalized_vector(t_vec3 vec);
int			validate_plane_normal(t_vec3 *normal);

/* Memory management utilities */
void		ft_free(void **ptr);
void		ft_free_array(char ***tokens);
void		ft_free_scene(t_scene **scene);
void		free_tokens(char **tokens);
int			is_empty_line(const char *line);

/* Scene management functions */
int			add_object_to_scene(t_scene *scene, int type, void *object_data);

#endif
