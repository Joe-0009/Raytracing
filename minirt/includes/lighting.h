#ifndef LIGHTING_H
# define LIGHTING_H

# include "scene.h"
# include "intersections.h"
# include "math_utils.h"
# include "color_utils.h"

/*
 * Lighting calculation functions
 */

/* Calculate ambient lighting component */
t_color3	calculate_ambient(const t_scene *scene, const t_hit *hit);

/* Calculate diffuse lighting component */
t_color3	calculate_diffuse(const t_scene *scene, const t_hit *hit);

/* Check if a point is in shadow from a light source */
int			is_in_shadow(const t_scene *scene, const t_vec3 point, const t_vec3 light_pos);

/* Main lighting calculation function */
t_color3	calculate_lighting(const t_scene *scene, const t_hit *hit);

#endif
