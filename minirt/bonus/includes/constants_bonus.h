#ifndef CONSTANTS_H
# define CONSTANTS_H

/* Mathematical constants */
# define EPSILON 0.0001
# define MIN_T 0.001
# define SHADOW_EPSILON 1e-6
# define EARLY_TERMINATION_DISTANCE 0.002

/* Lighting constants */
# define LIGHTENING_FACTOR 0.1
# define ATTENUATION_LINEAR 0.005
# define ATTENUATION_QUADRATIC 0.0005

/* Specular reflection constants */
# define SPECULAR_SHININESS 32.0
# define SPECULAR_INTENSITY 0.8

/* Texture constants */
# define CHECKERBOARD_SIZE 0.5
# define BUMP_SCALE 0.1
# define MAX_TEXTURE_SIZE 1024

/* Texture types */
# define TEXTURE_NONE 0
# define TEXTURE_CHECKERBOARD 1
# define TEXTURE_BUMP 2

/* Rendering constants */
# define DEFAULT_SKY_COLOR_R 0
# define DEFAULT_SKY_COLOR_G 0
# define DEFAULT_SKY_COLOR_B 0

#endif
