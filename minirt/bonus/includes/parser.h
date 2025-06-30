#ifndef PARSER_H
# define PARSER_H

# include "scene_math.h"

typedef struct s_parser
{
	char	*line;
	char	**tokens;
	int		line_count;
	int		has_camera;
}			t_parser;

# define ERR_AMBIENT_FORMAT "Error: Invalid ambient lighting format\n"
# define ERR_CAMERA_FORMAT "Error: Invalid camera format\n"
# define ERR_LIGHT_FORMAT "Error: Invalid light format\n"
# define ERR_SPHERE_FORMAT "Error: Invalid sphere format\n"
# define ERR_PLANE_FORMAT "Error: Invalid plane format\n"
# define ERR_CYLINDER_FORMAT "Error: Invalid cylinder format\n"
# define ERR_CONE_FORMAT "Error: Invalid cone format\n"
# define ERR_UNKNOWN_ELEMENT "Error: Unknown element in scene file\n"
# define ERR_DUPLICATE_ELEMENT "Error: Duplicate unique element in scene file\n"
# define ERR_MISSING_ELEMENT "Error: Required element missing in scene file\n"
# define ERR_VECTOR_FORMAT "Error: Invalid vector format\n"
# define ERR_COLOR_FORMAT "Error: Invalid color format\n"
# define ERR_VALUE_RANGE "Error: Value out of allowed range\n"
# define ERR_FILE_EXTENSION "Error: File must have .rt extension\n"
# define ERR_FILE_ACCESS "Error: Could not open file %s\n"
# define ERR_UNKNOWN_IDENTIFIER "Error: Line %d: Unknown identifier '%s'\n"
# define ERR_MEMORY "Error: Memory allocation failed\n"

/* Additional error messages for printf statements */
# define ERR_AMBIENT_ALREADY_DEFINED "Error: Ambient lighting already defined\n"
# define ERR_AMBIENT_RATIO_RANGE "Error: Ambient ratio must be in [0.0, 1.0]\n"
# define ERR_AMBIENT_COLOR_INVALID "Error: Invalid color for ambient lighting\n"
# define ERR_AMBIENT_TOO_MANY_ARGS "Too many arguments for ambient\n"
# define ERR_LIGHT_ALREADY_DEFINED "Error: Light source already defined\n"
# define ERR_LIGHT_BRIGHTNESS_RANGE "Error: Light brightness in [0.0, 1.0]\n"
# define ERR_LIGHT_COLOR_INVALID "Error: Invalid color for light source\n"
# define ERR_LIGHT_TOO_MANY_ARGS "Too many arguments for light source\n"
# define ERR_SPHERE_COLOR_INVALID "Error: Invalid color for sphere\n"
# define ERR_SPHERE_TOO_MANY_ARGS "Too many arguments for sphere\n"
# define ERR_SPHERE_DIAMETER_POSITIVE "Sphere diameter must be positive\n"
# define ERR_PLANE_COLOR_INVALID "Error: Invalid color for plane\n"
# define ERR_PLANE_TOO_MANY_ARGS "Too many arguments for plane\n"
# define ERR_PLANE_NORMAL_ZERO "Error: Plane normal vector cannot be (0,0,0)\n"
# define ERR_PLANE_NORMAL_NOT_NORMALIZED "Error: Plane normal not normalized\n"
# define ERR_CYLINDER_COLOR_INVALID "Error: Invalid cylinder color\n"
# define ERR_CYLINDER_TOO_MANY_ARGS "Too many arguments for cylinder\n"
# define ERR_CYLINDER_AXIS_ZERO "Error: Cylinder axis cannot be zero\n"
# define ERR_CYLINDER_AXIS_NOT_NORMALIZED "Error: Cylinder axis not normalized\n"
# define ERR_CYLINDER_DIMS_POSITIVE "Error: Cylinder dims must be positive\n"
# define ERR_CYLINDER_HEIGHT_NEGATIVE "Error: Cylinder height negative\n"
# define ERR_CONE_COLOR_INVALID "Error: Invalid color for cone\n"
# define ERR_CONE_TOO_MANY_ARGS "Too many arguments for cone\n"
# define ERR_CONE_DIMS_POSITIVE "Cone angle and height must be positive\n"
# define ERR_CONE_ANGLE_TOO_LARGE "Error: Cone angle must be <= 180 deg\n"
# define ERR_CAMERA_FOV_RANGE "Camera FOV must be in [0, 180] degrees\n"
# define ERR_SCENE_NO_CAMERA "Error: Camera not defined\n"
# define ERR_SCENE_NO_AMBIENT "Error: Ambient lighting not defined\n"
# define ERR_SCENE_NO_LIGHT "Error: Light source not defined\n"
# define ERR_CAMERA_ORIENTATION_ZERO "Error: Camera orientation zero\n"
# define ERR_SPHERE_INVALID "Error: Invalid sphere\n"
# define ERR_CYLINDER_INVALID "Error: Invalid cylinder\n"
# define ERR_PLANE_INVALID "Error: Invalid plane\n"
# define ERR_SCENE_NO_CAMERA_RENDER "Error: No camera in scene\n"

/* Warning messages */
# define WARN_SPHERE_DIAMETER_SMALL "Warning: Very small sphere diameter\n"
# define WARN_SPHERE_DIAMETER_VERY_SMALL "Warning: Sphere diameter is very small\n"
# define WARN_CYLINDER_DIAMETER_SMALL "Warning: Very small cylinder diameter\n"
# define WARN_CYLINDER_HEIGHT_SMALL "Warning: Very small cylinder height\n"
# define WARN_CYLINDER_DIMS_SMALL "Warning: Cylinder dimensions are very small\n"
# define WARN_CONE_ANGLE_SMALL "Warning: Small cone angle (%.6f rad)\n"
# define WARN_CONE_HEIGHT_SMALL "Warning: Small cone height (%.6f)\n"
# define WARN_POSITION_FAR "Warning: %s far from origin (%.2f, %.2f, %.2f)\n"

/* Format strings */
# define FMT_AMBIENT_EXPECTED "Expected format: A ratio r,g,b\n"
# define FMT_LIGHT_EXPECTED "Expected format: L x,y,z brightness r,g,b\n"
# define FMT_SPHERE_EXPECTED "Expected format: sp x,y,z diameter r,g,b\n"
# define FMT_PLANE_EXPECTED "Expected format: pl x,y,z nx,ny,nz r,g,b\n"
# define FMT_CYLINDER_EXPECTED "Expected: cy x,y,z nx,ny,nz diameter height r,g,b\n"
# define FMT_CONE_EXPECTED "Expected format: cn x,y,z axis_x,y,z angle height r,g,b\n"
# define FMT_CAMERA_EXPECTED "Expected format: C x,y,z nx,ny,nz fov\n"

/* Function prototypes */
/* File and scene loading */
t_scene		*parse_scene_file(char *filename);
int			validate_scene(t_scene *scene);
int			validate_scene_rendering(t_scene *scene);

/* Element parsing functions */
int			parse_ambient(char **tokens, t_scene *scene);
int			parse_light(char **tokens, t_scene *scene);
int			parse_camera(char **tokens, t_scene *scene);
int			parse_sphere(char **tokens, t_scene *scene);
int			parse_plane(char **tokens, t_scene *scene);
int			parse_cylinder(char **tokens, t_scene *scene);
int			parse_cone(char **tokens, t_scene *scene);

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
int			validate_sphere_diameter(double diameter);
int			validate_cylinder_dimensions(double diameter, double height);
int			validate_cone_dimensions(double angle, double height);
int			validate_plane_normal(t_vec3 *normal);
void		free_tokens(char **tokens);

/* Scene management functions */
int			add_object_to_scene(t_scene *scene, int type, void *object_data);

#endif
