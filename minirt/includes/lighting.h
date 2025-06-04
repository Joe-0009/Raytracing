#ifndef LIGHTING_H
# define LIGHTING_H

# include "scene.h"
# include "intersections.h"
# include "math_utils.h"
# include "color_utils.h"

t_color3	calculate_ambient(const t_scene *scene, const t_hit *hit);
t_color3	calculate_diffuse(const t_scene *scene, const t_hit *hit);
int			is_in_shadow(const t_scene *scene, const t_vec3 point, const t_vec3 light_pos);
t_color3	calculate_lighting(const t_scene *scene, const t_hit *hit);

#endif
