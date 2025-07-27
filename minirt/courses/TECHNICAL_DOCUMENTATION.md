# 🌟 miniRT - Complete Technical Documentation

## Ray Tracing Engine Implementation Guide

*A comprehensive guide to understanding the mathematical foundations, architectural decisions, and implementation details of a complete ray tracing engine.*

---

## 📋 Table of Contents

1. [Project Overview](#project-overview)
2. [Mathematical Foundations](#mathematical-foundations)
3. [Architecture & Design Patterns](#architecture--design-patterns)
4. [Core Data Structures](#core-data-structures)
5. [Ray-Object Intersection Algorithms](#ray-object-intersection-algorithms)
6. [Lighting Models](#lighting-models)
7. [Transformation Systems](#transformation-systems)
8. [Texture & Bump Mapping](#texture--bump-mapping)
9. [Performance Optimizations](#performance-optimizations)
10. [File Structure Analysis](#file-structure-analysis)
11. [API Reference](#api-reference)
12. [Implementation Diagrams](#implementation-diagrams)

---

## 🎯 Project Overview

### What is Ray Tracing?

Ray tracing is a rendering technique that simulates the behavior of light rays to generate photorealistic images. Unlike rasterization (used in real-time graphics), ray tracing traces the path of light rays backward from the camera through each pixel into the scene.

```
Camera → Pixel → Ray → Object Intersection → Lighting Calculation → Color
```

### Project Goals

- **Primary**: Implement a functional ray tracer in C
- **Educational**: Master 3D mathematics and computer graphics concepts
- **Technical**: Build a clean, extensible codebase following 42 standards

---

## 🧮 Mathematical Foundations

### 1. Vector Mathematics

#### Vector Structure
```c
typedef struct s_vec3
{
    double x;
    double y;
    double z;
} t_vec3;
```

#### Core Vector Operations

**Dot Product** - Measures similarity between vectors
```
v1 · v2 = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z
```

**Cross Product** - Finds perpendicular vector
```
v1 × v2 = (v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x)
```

**Vector Normalization**
```
normalized(v) = v / |v|
where |v| = √(v.x² + v.y² + v.z²)
```

### 2. Ray Mathematics

#### Ray Definition
A ray is defined by an origin point and a direction vector:
```
Ray(t) = Origin + t * Direction
where t ≥ 0
```

#### Ray Structure
```c
typedef struct s_ray
{
    t_vec3 origin;      // Starting point
    t_vec3 direction;   // Normalized direction vector
    double t_min;       // Minimum intersection distance
    double t_max;       // Maximum intersection distance
} t_ray;
```

### 3. Matrix Mathematics

#### 4x4 Transformation Matrix
```c
typedef struct s_matrix4
{
    double m[4][4];
} t_matrix4;
```

#### Matrix Operations

**Matrix Multiplication**
```
C[i][j] = Σ(k=0 to 3) A[i][k] * B[k][j]
```

**Translation Matrix**
```
T = | 1  0  0  tx |
    | 0  1  0  ty |
    | 0  0  1  tz |
    | 0  0  0  1  |
```

**Rotation Matrix (around X-axis)**
```
Rx = | 1   0      0     0 |
     | 0  cos(θ) -sin(θ) 0 |
     | 0  sin(θ)  cos(θ) 0 |
     | 0   0      0     1 |
```

---

## 🏗️ Architecture & Design Patterns

### Project Structure

```
miniRT/
├── mandatory/          # Core implementation
│   ├── includes/       # Header files
│   ├── src/
│   │   ├── events/    # User interaction
│   │   ├── maths/     # Mathematical operations
│   │   ├── parser/    # Scene file parsing
│   │   └── render/    # Rendering pipeline
├── bonus/             # Extended features
│   └── [same structure as mandatory]
└── prebuilt_libs/     # External libraries
```

### Design Patterns Used

1. **Strategy Pattern** - Different intersection algorithms for each object type
2. **Factory Pattern** - Object creation based on scene file parsing
3. **Observer Pattern** - Event handling system
4. **Template Method** - Unified transformation pipeline

---

## 📊 Core Data Structures

### Scene Representation

```c
typedef struct s_scene
{
    t_camera    camera;
    t_ambient   ambient;
    t_light     *lights;
    t_object    *objects;
    int         num_lights;
    int         num_objects;
    int         selected_obj;    // For interactive mode
} t_scene;
```

### Object System

```c
typedef enum e_object_type
{
    SPHERE,
    PLANE,
    CYLINDER,
    CONE        // Bonus
} t_object_type;

typedef struct s_object
{
    t_object_type   type;
    t_color3        color;
    union {
        t_sphere    sphere;
        t_plane     plane;
        t_cylinder  cylinder;
        t_cone      cone;
    } data;
} t_object;
```

### Geometric Primitives

#### Sphere
```c
typedef struct s_sphere
{
    t_vec3      center;
    double      radius;
    t_surface_map texture;     // Bonus: texture mapping
    t_surface_map bump_map;    // Bonus: bump mapping
    int         checkerboard;  // Bonus: procedural pattern
} t_sphere;
```

#### Plane
```c
typedef struct s_plane
{
    t_vec3      point;      // Any point on the plane
    t_vec3      normal;     // Normalized normal vector
} t_plane;
```

#### Cylinder
```c
typedef struct s_cylinder
{
    t_vec3      center;     // Center of the base
    t_vec3      axis;       // Normalized axis direction
    double      diameter;
    double      height;
} t_cylinder;
```

#### Cone (Bonus)
```c
typedef struct s_cone
{
    t_vec3      vertex;     // Apex of the cone
    t_vec3      axis;       // Normalized axis direction
    double      angle;      // Half-angle of the cone
    double      height;
} t_cone;
```

### Camera System

```c
typedef struct s_camera
{
    t_vec3      position;       // Camera position in world space
    t_vec3      orientation;    // Normalized looking direction
    double      fov;            // Horizontal field of view (degrees)
} t_camera;
```

### Lighting System

```c
typedef struct s_light
{
    t_vec3      position;
    double      brightness;     // [0.0, 1.0]
    t_color3    color;         // RGB color (bonus)
} t_light;

typedef struct s_ambient
{
    double      ratio;         // [0.0, 1.0]
    t_color3    color;         // RGB color
} t_ambient;
```

---

## 🎯 Ray-Object Intersection Algorithms

### 1. Ray-Sphere Intersection

#### Mathematical Foundation
A sphere is defined by: `|P - C|² = r²`
Ray equation: `P(t) = O + t*D`

Substituting: `|O + t*D - C|² = r²`

Expanding: `(O + t*D - C) · (O + t*D - C) = r²`

This gives us a quadratic equation: `at² + bt + c = 0`

```c
// Coefficients calculation
t_vec3  oc = vec3_sub(ray.origin, sphere.center);
double  a = vec3_dot(ray.direction, ray.direction);
double  b = 2.0 * vec3_dot(oc, ray.direction);
double  c = vec3_dot(oc, oc) - sphere.radius * sphere.radius;

// Discriminant
double discriminant = b * b - 4 * a * c;
if (discriminant < 0)
    return false;  // No intersection

// Solutions
double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
double t2 = (-b + sqrt(discriminant)) / (2.0 * a);
```

#### Implementation
```c
int intersect_sphere(const t_ray *ray, const t_sphere *sphere, t_hit *hit)
{
    t_vec3  oc = vec3_sub(ray->origin, sphere->center);
    double  a = vec3_dot(ray->direction, ray->direction);
    double  b = 2.0 * vec3_dot(oc, ray->direction);
    double  c = vec3_dot(oc, oc) - sphere->radius * sphere->radius;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return (0);

    double t = (-b - sqrt(discriminant)) / (2.0 * a);
    if (t < ray->t_min || t > ray->t_max)
    {
        t = (-b + sqrt(discriminant)) / (2.0 * a);
        if (t < ray->t_min || t > ray->t_max)
            return (0);
    }

    hit->t = t;
    hit->point = vec3_add(ray->origin, vec3_mult(ray->direction, t));
    hit->normal = vec3_normalize(vec3_sub(hit->point, sphere->center));

    return (1);
}
```

### 2. Ray-Plane Intersection

#### Mathematical Foundation
Plane equation: `N · (P - P₀) = 0`
Ray equation: `P(t) = O + t*D`

Substituting: `N · (O + t*D - P₀) = 0`
Solving for t: `t = N · (P₀ - O) / (N · D)`

```c
int intersect_plane(const t_ray *ray, const t_plane *plane, t_hit *hit)
{
    double  denom = vec3_dot(plane->normal, ray->direction);

    // Parallel to plane
    if (fabs(denom) < EPSILON)
        return (0);

    t_vec3  p0_to_origin = vec3_sub(plane->point, ray->origin);
    double  t = vec3_dot(p0_to_origin, plane->normal) / denom;

    if (t < ray->t_min || t > ray->t_max)
        return (0);

    hit->t = t;
    hit->point = vec3_add(ray->origin, vec3_mult(ray->direction, t));
    hit->normal = plane->normal;

    return (1);
}
```

### 3. Ray-Cylinder Intersection

#### Mathematical Foundation
Cylinder: `|(P - C) - ((P - C) · A) * A|² = r²`

This is more complex and involves:
1. Intersecting with infinite cylinder
2. Checking height constraints
3. Handling caps (if finite)

```c
int intersect_cylinder(const t_ray *ray, const t_cylinder *cylinder, t_hit *hit)
{
    t_vec3  oc = vec3_sub(ray->origin, cylinder->center);
    t_vec3  d_cross_axis = vec3_cross(ray->direction, cylinder->axis);
    t_vec3  oc_cross_axis = vec3_cross(oc, cylinder->axis);

    double  a = vec3_dot(d_cross_axis, d_cross_axis);
    double  b = 2.0 * vec3_dot(d_cross_axis, oc_cross_axis);
    double  c = vec3_dot(oc_cross_axis, oc_cross_axis) -
                (cylinder->diameter / 2.0) * (cylinder->diameter / 2.0);

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return (0);

    // Find valid intersection within height constraints
    // ... (implementation details)
}
```

### 4. Ray-Cone Intersection (Bonus)

#### Mathematical Foundation
Cone equation in parametric form is complex, involving:
- Vertex position
- Axis direction
- Half-angle
- Height constraints

---

## 💡 Lighting Models

### 1. Phong Illumination Model

The complete Phong model consists of three components:

```
I = I_ambient + I_diffuse + I_specular
```

#### Ambient Lighting
```
I_ambient = k_a * I_a
```
Where:
- `k_a` = ambient reflection coefficient
- `I_a` = ambient light intensity

#### Diffuse Lighting (Lambert)
```
I_diffuse = k_d * I_l * (N · L)
```
Where:
- `k_d` = diffuse reflection coefficient
- `I_l` = light intensity
- `N` = surface normal
- `L` = light direction

#### Specular Lighting
```
I_specular = k_s * I_l * (R · V)^n
```
Where:
- `k_s` = specular reflection coefficient
- `R` = reflected light direction
- `V` = view direction
- `n` = shininess factor

### Implementation

```c
t_color3 calculate_phong_lighting(const t_scene *scene, const t_hit *hit, t_vec3 view_dir)
{
    t_color3 result = vec3_mult(scene->ambient.color, scene->ambient.ratio);

    for (int i = 0; i < scene->num_lights; i++)
    {
        t_vec3 light_dir = vec3_normalize(vec3_sub(scene->lights[i].position, hit->point));

        // Shadow test
        if (is_in_shadow(scene, hit->point, light_dir))
            continue;

        // Diffuse component
        double diff = fmax(0.0, vec3_dot(hit->normal, light_dir));
        t_color3 diffuse = vec3_mult(scene->lights[i].color,
                                   scene->lights[i].brightness * diff);

        // Specular component (bonus)
        t_vec3 reflect_dir = reflect(vec3_mult(light_dir, -1), hit->normal);
        double spec = pow(fmax(0.0, vec3_dot(view_dir, reflect_dir)), 32);
        t_color3 specular = vec3_mult(vec3_create(1, 1, 1), spec * 0.5);

        result = vec3_add(result, vec3_add(diffuse, specular));
    }

    return clamp_color(result);
}
```

### 2. Shadow Calculation

```c
int is_in_shadow(const t_scene *scene, t_vec3 point, t_vec3 light_dir)
{
    t_ray shadow_ray;
    shadow_ray.origin = vec3_add(point, vec3_mult(light_dir, EPSILON));
    shadow_ray.direction = light_dir;
    shadow_ray.t_min = EPSILON;
    shadow_ray.t_max = INFINITY;

    t_hit temp_hit;
    return find_closest_intersection(scene, &shadow_ray, &temp_hit);
}
```

---

## 🔄 Transformation Systems

### 1. Direct Matrix Operations (Old System)

```c
void scene_rotate_object_old(t_scene *scene, int obj_index, t_vec3 rotation)
{
    // Create separate matrices each time
    t_matrix4 rotation_x = matrix4_rotation_x(rotation.x);
    t_matrix4 rotation_y = matrix4_rotation_y(rotation.y);
    t_matrix4 rotation_z = matrix4_rotation_z(rotation.z);

    // Combine matrices
    t_matrix4 combined = matrix4_multiply(rotation_z, rotation_y);
    combined = matrix4_multiply(combined, rotation_x);

    // Apply to object properties directly
    // ... object-specific transformations
}
```

### 2. Unified Matrix System (New System)

#### Transform Structure
```c
typedef struct s_transform
{
    t_vec3      translation;    // Translation vector
    t_vec3      rotation;       // Euler angles (X, Y, Z)
    t_vec3      scale;          // Scale factors
    t_matrix4   matrix;         // Combined transformation matrix
} t_transform;
```

#### Benefits of Unified System
1. **Matrix Caching** - Avoids redundant calculations
2. **Consistency** - Same pipeline for all objects
3. **Composability** - Easy to combine transformations
4. **Performance** - Optimized matrix operations

#### Implementation
```c
void transform_update_matrix(t_transform *transform)
{
    transform->matrix = matrix4_transform(
        transform->translation,
        transform->rotation,
        transform->scale
    );
}

void scene_rotate_object(t_scene *scene, int obj_index, t_vec3 rotation)
{
    t_transform transform = create_rotation_transform(rotation);
    apply_transform_to_object(&scene->objects[obj_index], &transform);
}
```

### 3. Rodrigues' Rotation Formula

For arbitrary axis rotation:

```
R = I + sin(θ)K + (1-cos(θ))K²
```

Where K is the skew-symmetric matrix of the rotation axis.

```c
t_matrix4 matrix4_rotation_axis(t_vec3 axis, double angle)
{
    t_vec3 k = vec3_normalize(axis);
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    double one_minus_cos = 1.0 - cos_a;

    t_matrix4 m = matrix4_identity();

    // Diagonal elements
    m.m[0][0] = cos_a + k.x * k.x * one_minus_cos;
    m.m[1][1] = cos_a + k.y * k.y * one_minus_cos;
    m.m[2][2] = cos_a + k.z * k.z * one_minus_cos;

    // Off-diagonal elements
    m.m[0][1] = k.x * k.y * one_minus_cos - k.z * sin_a;
    m.m[0][2] = k.x * k.z * one_minus_cos + k.y * sin_a;
    m.m[1][0] = k.y * k.x * one_minus_cos + k.z * sin_a;
    m.m[1][2] = k.y * k.z * one_minus_cos - k.x * sin_a;
    m.m[2][0] = k.z * k.x * one_minus_cos - k.y * sin_a;
    m.m[2][1] = k.z * k.y * one_minus_cos + k.x * sin_a;

    return m;
}
```

---

## 🎨 Texture & Bump Mapping (Bonus)

### 1. UV Coordinate Mapping

#### Sphere UV Mapping
```c
t_uv get_sphere_uv(t_vec3 point, t_vec3 center)
{
    t_vec3 n = vec3_normalize(vec3_sub(point, center));

    double theta = atan2(n.z, n.x);           // Azimuth
    double phi = acos(n.y);                   // Polar angle

    t_uv uv;
    uv.u = (theta + M_PI) / (2.0 * M_PI);     // [0, 1]
    uv.v = phi / M_PI;                        // [0, 1]

    return uv;
}
```

#### Cylindrical UV Mapping
```c
t_uv get_cylinder_uv(t_vec3 point, t_cylinder *cylinder)
{
    t_vec3 local_point = vec3_sub(point, cylinder->center);

    // Project onto plane perpendicular to axis
    double height_along_axis = vec3_dot(local_point, cylinder->axis);
    t_vec3 radial = vec3_sub(local_point,
                            vec3_mult(cylinder->axis, height_along_axis));

    t_uv uv;
    uv.u = (atan2(radial.z, radial.x) + M_PI) / (2.0 * M_PI);
    uv.v = height_along_axis / cylinder->height;

    return uv;
}
```

### 2. Texture Application

```c
t_color3 apply_texture(const t_surface_map *texture, t_uv uv)
{
    if (!texture->is_active)
        return vec3_create(1, 1, 1);  // White default

    // Wrap UV coordinates
    uv.u = fmod(uv.u, 1.0);
    uv.v = fmod(uv.v, 1.0);
    if (uv.u < 0) uv.u += 1.0;
    if (uv.v < 0) uv.v += 1.0;

    // Sample texture
    int x = (int)(uv.u * texture->width) % texture->width;
    int y = (int)(uv.v * texture->height) % texture->height;

    int pixel = texture->data[y * texture->width + x];

    return vec3_create(
        ((pixel >> 16) & 0xFF) / 255.0,  // Red
        ((pixel >> 8) & 0xFF) / 255.0,   // Green
        (pixel & 0xFF) / 255.0           // Blue
    );
}
```

### 3. Bump Mapping

Bump mapping perturbs surface normals to simulate surface detail:

```c
t_vec3 apply_bump_mapping(t_vec3 normal, const t_surface_map *bump_map, t_uv uv)
{
    if (!bump_map->is_active)
        return normal;

    // Sample bump map
    t_color3 bump_sample = apply_texture(bump_map, uv);
    double height = (bump_sample.x + bump_sample.y + bump_sample.z) / 3.0;

    // Calculate gradient (simplified)
    double bump_strength = 0.1;
    t_vec3 perturbation = vec3_mult(normal, height * bump_strength);

    return vec3_normalize(vec3_add(normal, perturbation));
}
```

### 4. Checkerboard Pattern

```c
t_color3 get_checkerboard_color(t_uv uv, t_color3 base_color)
{
    int checker_u = (int)(uv.u * 10) % 2;
    int checker_v = (int)(uv.v * 10) % 2;

    if (checker_u ^ checker_v)  // XOR for checkerboard
        return base_color;
    else
        return vec3_mult(base_color, 0.5);  // Darker squares
}
```

---

## ⚡ Performance Optimizations

### 1. Compilation Flags

```makefile
CFLAGS = -Wall -Wextra -Werror -O3 -ffast-math -funroll-loops \
         -ftree-vectorize -fomit-frame-pointer -march=native \
         -mtune=native -flto -finline-functions
```

**Explanation:**
- `-O3`: Maximum optimization
- `-ffast-math`: Aggressive floating-point optimizations
- `-funroll-loops`: Unroll loops for better performance
- `-march=native`: Optimize for current CPU architecture
- `-flto`: Link-time optimization

### 2. Mathematical Optimizations

#### Fast Vector Operations
```c
// Optimized dot product
static inline double vec3_dot_fast(t_vec3 a, t_vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Fast inverse square root (approximation)
static inline double fast_inv_sqrt(double x)
{
    return 1.0 / sqrt(x);  // Let compiler optimize
}
```

#### Early Ray Termination
```c
int find_closest_intersection(const t_scene *scene, t_ray *ray, t_hit *closest_hit)
{
    int hit_found = 0;
    double closest_t = ray->t_max;

    for (int i = 0; i < scene->num_objects; i++)
    {
        t_hit temp_hit;
        if (intersect_object(ray, &scene->objects[i], &temp_hit))
        {
            if (temp_hit.t < closest_t)
            {
                closest_t = temp_hit.t;
                *closest_hit = temp_hit;
                hit_found = 1;
                ray->t_max = temp_hit.t;  // Early termination
            }
        }
    }

    return hit_found;
}
```

### 3. Memory Access Patterns

#### Structure Alignment
```c
// Align structures for better cache performance
typedef struct s_vec3
{
    double x;
    double y;
    double z;
    // Implicit padding for alignment
} __attribute__((aligned(32))) t_vec3;
```

#### Cache-Friendly Data Layout
```c
// Store frequently accessed data together
typedef struct s_scene
{
    // Hot data first
    t_camera    camera;
    int         num_objects;
    int         num_lights;

    // Cold data last
    t_object    *objects;
    t_light     *lights;
} t_scene;
```

---

## 📁 File Structure Analysis

### Mandatory Part Headers

#### `includes/math_utils.h`
```c
#ifndef MATH_UTILS_H
# define MATH_UTILS_H

# include <math.h>

// Vector types
typedef struct s_vec3 { double x, y, z; } t_vec3;
typedef struct s_matrix4 { double m[4][4]; } t_matrix4;

// Vector operations
t_vec3    vec3_create(double x, double y, double z);
t_vec3    vec3_add(t_vec3 v1, t_vec3 v2);
t_vec3    vec3_sub(t_vec3 v1, t_vec3 v2);
t_vec3    vec3_mult(t_vec3 v, double t);
t_vec3    vec3_normalize(t_vec3 v);
double    vec3_dot(t_vec3 v1, t_vec3 v2);
t_vec3    vec3_cross(t_vec3 v1, t_vec3 v2);

// Matrix operations
t_matrix4 matrix4_identity(void);
t_matrix4 matrix4_multiply(t_matrix4 a, t_matrix4 b);
t_matrix4 matrix4_translation(t_vec3 translation);
t_matrix4 matrix4_rotation_x(double angle);
t_matrix4 matrix4_rotation_y(double angle);
t_matrix4 matrix4_rotation_z(double angle);

// Advanced matrix operations
t_matrix4 matrix4_rotation_axis(t_vec3 axis, double angle);
t_matrix4 matrix4_transform(t_vec3 translation, t_vec3 rotation, t_vec3 scale);

#endif
```

#### `includes/scene.h`
```c
#ifndef SCENE_H
# define SCENE_H

# include "math_utils.h"

// Object types
typedef enum e_object_type
{
    SPHERE,
    PLANE,
    CYLINDER
} t_object_type;

// Scene elements
typedef struct s_camera
{
    t_vec3  position;
    t_vec3  orientation;
    double  fov;
} t_camera;

typedef struct s_light
{
    t_vec3  position;
    double  brightness;
    t_vec3  color;
} t_light;

typedef struct s_sphere
{
    t_vec3  center;
    double  radius;
} t_sphere;

typedef struct s_plane
{
    t_vec3  point;
    t_vec3  normal;
} t_plane;

typedef struct s_cylinder
{
    t_vec3  center;
    t_vec3  axis;
    double  diameter;
    double  height;
} t_cylinder;

typedef struct s_object
{
    t_object_type   type;
    t_vec3          color;
    union {
        t_sphere    sphere;
        t_plane     plane;
        t_cylinder  cylinder;
    } data;
} t_object;

typedef struct s_scene
{
    t_camera    camera;
    t_light     *lights;
    t_object    *objects;
    int         num_lights;
    int         num_objects;
} t_scene;

#endif
```

### Bonus Part Extensions

#### `includes/scene_bonus.h`
```c
// Extends mandatory scene.h with:

// Additional object type
typedef enum e_object_type
{
    SPHERE,
    PLANE,
    CYLINDER,
    CONE        // Bonus addition
} t_object_type;

// Texture mapping
typedef struct s_surface_map
{
    int     *data;
    int     width;
    int     height;
    int     is_active;
} t_surface_map;

// Enhanced sphere with textures
typedef struct s_sphere
{
    t_vec3          center;
    double          radius;
    t_surface_map   texture;        // Color texture
    t_surface_map   bump_map;       // Bump mapping
    int             checkerboard;   // Procedural pattern
} t_sphere;

// Cone geometry
typedef struct s_cone
{
    t_vec3  vertex;
    t_vec3  axis;
    double  angle;
    double  height;
} t_cone;
```

#### `includes/events_bonus.h`
```c
// Interactive transformation system

typedef struct s_transform
{
    t_vec3      translation;
    t_vec3      rotation;
    t_vec3      scale;
    t_matrix4   matrix;
} t_transform;

// Transform operations
t_transform create_unified_transform(t_vec3 translation, t_vec3 rotation, t_vec3 scale);
void        apply_transform_to_object(t_object *object, t_transform *transform);

// Scene transformation utilities
void        scene_translate_object(t_scene *scene, int obj_index, t_vec3 delta);
void        scene_rotate_object(t_scene *scene, int obj_index, t_vec3 rotation);
void        scene_scale_object(t_scene *scene, int obj_index, double scale);
```

---

## 🔧 API Reference

### Core Rendering Functions

#### `raytrace.c`
```c
t_color3 trace_ray(const t_scene *scene, const t_ray *ray, int depth);
int      find_closest_intersection(const t_scene *scene, t_ray *ray, t_hit *hit);
```

#### `intersections.c`
```c
int intersect_sphere(const t_ray *ray, const t_sphere *sphere, t_hit *hit);
int intersect_plane(const t_ray *ray, const t_plane *plane, t_hit *hit);
int intersect_cylinder(const t_ray *ray, const t_cylinder *cylinder, t_hit *hit);
int intersect_cone(const t_ray *ray, const t_cone *cone, t_hit *hit);  // Bonus
```

#### `lighting.c`
```c
t_color3 calculate_lighting(const t_scene *scene, const t_hit *hit, t_vec3 view_dir);
t_color3 calculate_phong_lighting(const t_scene *scene, const t_hit *hit, t_vec3 view_dir);  // Bonus
int      is_in_shadow(const t_scene *scene, t_vec3 point, t_vec3 light_dir);
```

### Mathematical Functions

#### `vector_ops.c`
```c
t_vec3  vec3_create(double x, double y, double z);
t_vec3  vec3_add(t_vec3 v1, t_vec3 v2);
t_vec3  vec3_sub(t_vec3 v1, t_vec3 v2);
t_vec3  vec3_mult(t_vec3 v, double scalar);
t_vec3  vec3_normalize(t_vec3 v);
double  vec3_dot(t_vec3 v1, t_vec3 v2);
t_vec3  vec3_cross(t_vec3 v1, t_vec3 v2);
double  vec3_length(t_vec3 v);
t_vec3  reflect(t_vec3 incident, t_vec3 normal);
```

#### `matrix_op.c`
```c
t_matrix4 matrix4_identity(void);
t_matrix4 matrix4_multiply(t_matrix4 a, t_matrix4 b);
t_matrix4 matrix4_translation(t_vec3 translation);
t_matrix4 matrix4_rotation_x(double angle);
t_matrix4 matrix4_rotation_y(double angle);
t_matrix4 matrix4_rotation_z(double angle);
t_matrix4 matrix4_scale(t_vec3 scale);
t_vec3    matrix4_transform_point(t_matrix4 m, t_vec3 point);
t_vec3    matrix4_transform_direction(t_matrix4 m, t_vec3 direction);
```

#### `matrix_advanced.c`
```c
t_matrix4 matrix4_rotation_axis(t_vec3 axis, double angle);
t_matrix4 matrix4_transform(t_vec3 translation, t_vec3 rotation, t_vec3 scale);
t_matrix4 matrix4_look_at(t_vec3 eye, t_vec3 target, t_vec3 up);
void      matrix4_decompose(t_matrix4 matrix, t_vec3 *translation, t_vec3 *rotation, t_vec3 *scale);
```

### Scene Management

#### `parser.c`
```c
t_scene *parse_scene_file(const char *filename);
int      parse_camera(char **tokens, t_scene *scene);
int      parse_light(char **tokens, t_scene *scene);
int      parse_sphere(char **tokens, t_scene *scene);
int      parse_plane(char **tokens, t_scene *scene);
int      parse_cylinder(char **tokens, t_scene *scene);
int      parse_cone(char **tokens, t_scene *scene);    // Bonus
```

#### `events.c`
```c
int  key_handler(int keycode, t_vars *vars);
int  close_window_x(t_vars *vars);
int  close_window_esc(int keycode, t_vars *vars);
void handle_camera_movement(int keycode, t_scene *scene);
void handle_object_transforms(int keycode, t_scene *scene);  // Bonus
```

### Texture System (Bonus)

#### `texture_load.c`
```c
int load_texture(void *mlx, const char *filename, t_surface_map *texture);
int load_scene_textures(void *mlx, t_scene *scene);
```

#### `texture_apply.c`
```c
t_uv     get_sphere_uv(t_vec3 point, t_vec3 center);
t_uv     get_cylinder_uv(t_vec3 point, t_cylinder *cylinder);
t_color3 apply_texture(const t_surface_map *texture, t_uv uv);
t_vec3   apply_bump_mapping(t_vec3 normal, const t_surface_map *bump_map, t_uv uv);
t_color3 get_checkerboard_color(t_uv uv, t_color3 base_color);
```

---

## 📊 Implementation Diagrams

### 1. Ray Tracing Pipeline

```
┌─────────────────┐
│   Camera Setup  │
│  - Position     │
│  - Orientation  │
│  - FOV          │
└─────────┬───────┘
          │
          ▼
┌─────────────────┐
│   For Each Pixel│
│  - Calculate    │
│    ray direction│
└─────────┬───────┘
          │
          ▼
┌─────────────────┐
│ Ray-Scene       │
│ Intersection    │
│ - Test all      │
│   objects       │
│ - Find closest  │
└─────────┬───────┘
          │
          ▼
┌─────────────────┐
│   Lighting      │
│   Calculation   │
│ - Ambient       │
│ - Diffuse       │
│ - Specular      │
│ - Shadows       │
└─────────┬───────┘
          │
          ▼
┌─────────────────┐
│   Color Output  │
│ - Apply texture │
│ - Clamp values  │
│ - Set pixel     │
└─────────────────┘
```

### 2. Object Intersection Hierarchy

```
                    ┌─────────────────┐
                    │  intersect_     │
                    │  object()       │
                    └─────────┬───────┘
                              │
                ┌─────────────┼─────────────┐
                │             │             │
        ┌───────▼───────┐ ┌───▼───┐ ┌───────▼───────┐
        │ intersect_     │ │ ...   │ │ intersect_     │
        │ sphere()       │ │       │ │ cylinder()     │
        └───────┬───────┘ └───────┘ └───────┬───────┘
                │                           │
        ┌───────▼───────┐           ┌───────▼───────┐
        │ - Quadratic   │           │ - Infinite    │
        │   equation    │           │   cylinder    │
        │ - Discriminant│           │ - Height      │
        │ - Two roots   │           │   constraints │
        └───────────────┘           └───────────────┘
```

### 3. Transformation System Architecture

```
┌─────────────────────────────────────────────────────┐
│                 Transform System                    │
├─────────────────────────────────────────────────────┤
│                                                     │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  │
│  │Translation  │  │  Rotation   │  │   Scale     │  │
│  │   Matrix    │  │   Matrix    │  │   Matrix    │  │
│  └─────┬───────┘  └─────┬───────┘  └─────┬───────┘  │
│        │                │                │          │
│        └────────────────┼────────────────┘          │
│                         │                           │
│         ┌───────────────▼───────────────┐           │
│         │     Combined Transform        │           │
│         │         Matrix                │           │
│         └───────────────┬───────────────┘           │
│                         │                           │
│                         ▼                           │
│         ┌───────────────────────────────┐           │
│         │    Apply to Object            │           │
│         │  - Sphere center              │           │
│         │  - Plane point/normal         │           │
│         │  - Cylinder center/axis       │           │
│         └───────────────────────────────┘           │
└─────────────────────────────────────────────────────┘
```

### 4. Scene File Parsing Flow

```
┌─────────────────┐
│  .rt File       │
│                 │
│ A 0.2 255,255,255
│ C 0,0,20 0,0,-1 70
│ L 10,10,20 0.8 255,255,255
│ sp 0,0,0 10 255,0,0
│ pl 0,-8,0 0,1,0 100,100,100
│ cy 5,0,0 0,1,0 2 4 0,255,0
└─────────┬───────┘
          │
          ▼
┌─────────────────┐
│   Tokenizer     │
│ - Split lines   │
│ - Parse tokens  │
│ - Validate      │
└─────────┬───────┘
          │
          ▼
┌─────────────────┐     ┌─────────────────┐
│   Element       │────▶│   Scene         │
│   Parsers       │     │   Builder       │
│ - parse_camera  │     │ - Allocate      │
│ - parse_light   │     │ - Initialize    │
│ - parse_sphere  │     │ - Validate      │
│ - parse_plane   │     │                 │
│ - parse_cylinder│     │                 │
└─────────────────┘     └─────────┬───────┘
                                  │
                                  ▼
                        ┌─────────────────┐
                        │  Complete       │
                        │  Scene          │
                        │  Structure      │
                        └─────────────────┘
```

### 5. Memory Layout & Performance

```
┌─────────────────────────────────────────────────────┐
│                    Memory Layout                    │
├─────────────────────────────────────────────────────┤
│                                                     │
│  Hot Data (Frequently Accessed):                   │
│  ┌─────────────────────────────────────────────┐    │
│  │ Camera | Ambient | num_objects | num_lights │    │
│  └─────────────────────────────────────────────┘    │
│                                                     │
│  Object Array (Sequential Access):                 │
│  ┌───────┬───────┬───────┬───────┬───────┬─────┐    │
│  │Obj[0] │Obj[1] │Obj[2] │Obj[3] │Obj[4] │ ... │    │
│  └───────┴───────┴───────┴───────┴───────┴─────┘    │
│                                                     │
│  Light Array:                                      │
│  ┌────────┬────────┬────────┬────────┬──────┐       │
│  │Light[0]│Light[1]│Light[2]│Light[3]│ ...  │       │
│  └────────┴────────┴────────┴────────┴──────┘       │
│                                                     │
│  Cold Data (Rarely Accessed):                      │
│  ┌─────────────────────────────────────────────┐    │
│  │  Texture Data | Debug Info | Statistics    │    │
│  └─────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────┘
```

---

## 🏁 Conclusion

This miniRT implementation represents a comprehensive ray tracing engine that not only meets all the 42 subject requirements but extends far beyond them with professional-grade features:

### Key Achievements

1. **Mathematical Rigor**: Proper implementation of 3D mathematics, linear algebra, and computer graphics algorithms
2. **Clean Architecture**: Well-organized, modular codebase following software engineering best practices
3. **Performance Focus**: Optimized compilation flags and efficient algorithms
4. **Extensible Design**: Easy to add new object types, lighting models, and features
5. **Educational Value**: Serves as an excellent learning resource for computer graphics concepts

### Technical Highlights

- **Complete Phong lighting model** with ambient, diffuse, and specular components
- **Advanced transformation system** using unified matrix operations
- **Texture and bump mapping** for realistic surface details
- **Interactive controls** for real-time scene manipulation
- **Multiple object types** including complex primitives like cones
- **Procedural patterns** like checkerboard textures
- **Shadow calculation** for realistic lighting

### Future Enhancements

The current implementation provides an excellent foundation for advanced features:

- **Reflection and refraction** for mirrors and glass
- **Global illumination** using path tracing
- **Anti-aliasing** for smoother edges
- **Spatial acceleration structures** (BVH, k-d trees) for better performance
- **Volumetric rendering** for fog and smoke effects
- **Motion blur** and **depth of field**

This project demonstrates mastery of fundamental computer graphics concepts and provides a solid foundation for more advanced ray tracing techniques.

---

*Documentation created for miniRT project - A comprehensive ray tracing engine implementation*
