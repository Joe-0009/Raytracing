#ifndef SCENE_H
# define SCENE_H

# include "vector.h"

/* Object types */
# define SPHERE 1
# define PLANE 2
# define CYLINDER 3

/* Maximum objects in scene */
# define MAX_OBJECTS 100
# define MAX_LIGHTS 10

typedef struct s_ambient
{
    double  ratio;         /* Ambient lighting ratio */
    t_color3 color;        /* RGB color */
} t_ambient;

typedef struct s_camera
{
    t_point3 position;     /* Camera position */
    t_vec3   orientation;  /* Camera orientation vector (normalized) */
    double   fov;          /* Field of view in degrees */
} t_camera;

typedef struct s_light
{
    t_point3 position;     /* Light position */
    double   brightness;   /* Light brightness ratio */
    t_color3 color;        /* RGB color */
} t_light;

typedef struct s_sphere
{
    t_point3 center;       /* Sphere center */
    double   diameter;     /* Sphere diameter */
    t_color3 color;        /* RGB color */
} t_sphere;

typedef struct s_plane
{
    t_point3 point;        /* Point on the plane */
    t_vec3   normal;       /* Normal vector (normalized) */
    t_color3 color;        /* RGB color */
} t_plane;

typedef struct s_cylinder
{
    t_point3 center;       /* Cylinder base center */
    t_vec3   axis;         /* Cylinder axis (normalized) */
    double   diameter;     /* Cylinder diameter */
    double   height;       /* Cylinder height */
    t_color3 color;        /* RGB color */
} t_cylinder;

typedef struct s_object
{
    int type;              /* Object type (SPHERE, PLANE, CYLINDER) */
    union
    {
        t_sphere   sphere;
        t_plane    plane;
        t_cylinder cylinder;
    } data;
} t_object;

typedef struct s_scene
{
    t_ambient ambient;             /* Ambient lighting */
    t_camera  camera;              /* Camera settings */
    t_light   lights[MAX_LIGHTS];  /* Lights array */
    int       num_lights;          /* Number of lights */
    t_object  objects[MAX_OBJECTS]; /* Objects array */
    int       num_objects;         /* Number of objects */
} t_scene;

#endif
