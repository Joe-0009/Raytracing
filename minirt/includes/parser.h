#ifndef PARSER_H
# define PARSER_H

# include "scene.h"
#include <math.h>
/* Parse error codes and messages */
# define ERR_AMBIENT_FORMAT "Error: Invalid ambient lighting format\n"
# define ERR_CAMERA_FORMAT "Error: Invalid camera format\n"
# define ERR_LIGHT_FORMAT "Error: Invalid light format\n"
# define ERR_SPHERE_FORMAT "Error: Invalid sphere format\n"
# define ERR_PLANE_FORMAT "Error: Invalid plane format\n"
# define ERR_CYLINDER_FORMAT "Error: Invalid cylinder format\n"
# define ERR_UNKNOWN_ELEMENT "Error: Unknown element in scene file\n"
# define ERR_DUPLICATE_ELEMENT "Error: Duplicate unique element in scene file\n"
# define ERR_MISSING_ELEMENT "Error: Required element missing in scene file\n"
# define ERR_VECTOR_FORMAT "Error: Invalid vector format\n"
# define ERR_COLOR_FORMAT "Error: Invalid color format\n"
# define ERR_VALUE_RANGE "Error: Value out of allowed range\n"
# define ERR_FILE_EXTENSION "Error: File must have .rt extension\n"
# define ERR_FILE_ACCESS "Error: Could not open file %s\n"
# define ERR_UNKNOWN_IDENTIFIER "Error: Line %d: Unknown identifier '%s'\n"

/* Parser structure for tracking state during parsing */
typedef struct s_parser
{
    char    *line;
    char    **tokens;
    int     line_count;
    int     has_camera;
    int     has_ambient;
    int     has_light;
} t_parser;

/* Function prototypes */
/* File and scene loading */
t_scene *parse_scene_file(char *filename);
t_scene *parse_scene_file(char *filename);
int     validate_scene(t_scene *scene);
int     validate_scene_rendering(t_scene *scene);

/* Element parsing functions */
int     parse_ambient(char **tokens, t_scene *scene);
int     parse_camera(char **tokens, t_scene *scene);
int     parse_light(char **tokens, t_scene *scene);
int     parse_sphere(char **tokens, t_scene *scene);
int     parse_plane(char **tokens, t_scene *scene);
int     parse_cylinder(char **tokens, t_scene *scene);

/* Data type parsing */
int     parse_vector(char *str, t_vec3 *vec);
int     parse_color(char *str, t_color3 *color);
int     parse_double(char *str, double *value); 

/* Object validation */
int     validate_sphere(t_sphere *sphere);
int     validate_cylinder(t_cylinder *cylinder);
int     validate_plane(t_plane *plane);
int     validate_light(t_light *light);
int     validate_position(t_point3 pos, const char *type);
int     validate_non_zero_vector(t_vec3 vec);
int     validate_normalized_vector(t_vec3 vec);
int     validate_sphere_diameter(double diameter);
int     validate_cylinder_dimensions(double diameter, double height);
int     validate_plane_normal(t_vec3 *normal);
int     validate_light_brightness(double brightness);
void    free_tokens(char **tokens);

/* Scene management functions */
int     add_light_to_scene(t_scene *scene, t_light *light);
int     add_object_to_scene(t_scene *scene, int type, void *object_data);
int     validate_light_brightness(double brightness);
int     validate_sphere_diameter(double diameter);
int     validate_cylinder_dimensions(double diameter, double height);
int     validate_plane_normal(t_vec3 *normal);


#endif
