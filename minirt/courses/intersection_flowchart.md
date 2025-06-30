# Intersection System Flowchart

## Main Ray-Object Intersection Flow

```
┌─────────────────────────────────┐
│         START: trace_objects    │
│    (scene, ray, closest_hit)    │
└─────────────────┬───────────────┘
                  │
                  ▼
┌─────────────────────────────────┐
│      Initialize Variables       │
│   closest_hit->t = -1          │
│   hit_found = 0                │
│   i = 0                        │
└─────────────────┬───────────────┘
                  │
                  ▼
┌─────────────────────────────────┐
│    Loop Condition Check:        │
│    i < scene->num_objects?      │
└─────────────┬───────────────────┘
              │                   
              ▼                   
    ┌─────────────────┐           
    │       NO        │           
    │   Return        │           
    │   hit_found     │           
    └─────────────────┘           
              │ YES
              ▼
┌─────────────────────────────────┐
│     Check Object Type:          │
│   scene->objects[i].type        │
└─────────────┬───────────────────┘
              │
              ▼
    ┌─────────────────┐
    │     SPHERE?     │
    └─────┬─────┬─────┘
          │ YES │ NO
          ▼     ▼
┌───────────────┐    ┌─────────────────┐
│ trace_sphere  │    │     PLANE?      │
│ (&objects[i], │    └─────┬─────┬─────┘
│  ray,         │          │ YES │ NO
│  closest_hit, │          ▼     ▼
│  i)           │    ┌───────────────┐    ┌─────────────────┐
└───────┬───────┘    │ trace_plane   │    │   CYLINDER?     │
        │            │ (&objects[i], │    └─────┬─────┬─────┘
        ▼            │  ray,         │          │ YES │ NO
┌───────────────┐    │  closest_hit, │          ▼     ▼
│   Hit Found?  │    │  i)           │    ┌───────────────┐    ┌─────────────────┐
└───┬─────┬─────┘    └───────┬───────┘    │trace_cylinder │    │     CONE?       │
    │ YES │ NO              │            │ (&objects[i], │    └─────┬─────┬─────┘
    ▼     │                 ▼            │  ray,         │          │ YES │ NO
┌─────────┐│          ┌───────────────┐  │  closest_hit, │          ▼     ▼
│hit_found││          │   Hit Found?  │  │  i)           │    ┌───────────────┐
│   = 1   ││          └───┬─────┬─────┘  └───────┬───────┘    │  trace_cone   │
└─────────┘│              │ YES │ NO             │            │ (&objects[i], │
           │              ▼     │                ▼            │  ray,         │
           │        ┌─────────┐ │          ┌───────────────┐  │  closest_hit, │
           │        │hit_found│ │          │   Hit Found?  │  │  i)           │
           │        │   = 1   │ │          └───┬─────┬─────┘  └───────┬───────┘
           │        └─────────┘ │              │ YES │ NO             │
           │                    │              ▼     │                ▼
           └────────────────────┼────────┌─────────┐ │          ┌───────────────┐
                                │        │hit_found│ │          │   Hit Found?  │
                                │        │   = 1   │ │          └───┬─────┬─────┘
                                │        └─────────┘ │              │ YES │ NO
                                │                    │              ▼     │
                                └────────────────────┼────────┌─────────┐ │
                                                     │        │hit_found│ │
                                                     │        │   = 1   │ │
                                                     │        └─────────┘ │
                                                     │                    │
                                                     └────────────────────┘
                                                              │
                                                              ▼
                                                    ┌─────────────────┐
                                                    │   Increment i   │
                                                    │      i++        │
                                                    └─────────┬───────┘
                                                              │
                                                              ▼
                                                    ┌─────────────────┐
                                                    │   Loop Back     │
                                                    │   to Condition  │
                                                    └─────────────────┘
```

## Individual Intersection Function Flow

```
┌─────────────────────────────────┐
│     trace_[object_type]         │
│  (obj, ray, closest_hit, index) │
└─────────────────┬───────────────┘
                  │
                  ▼
┌─────────────────────────────────┐
│   Call intersect_[object_type]  │
│   (&obj->data.[type], ray,      │
│    closest_hit)                 │
└─────────────────┬───────────────┘
                  │
                  ▼
┌─────────────────────────────────┐
│    Intersection Found?          │
│    (function returns 1)         │
└─────────────┬───────────────────┘
              │
     ┌────────┴────────┐
     │ YES             │ NO
     ▼                 ▼
┌─────────────────┐  ┌─────────────────┐
│ Set obj_index:  │  │   Return 0      │
│ closest_hit->   │  │  (no hit)       │
│ obj_index = i   │  └─────────────────┘
└─────────┬───────┘
          │
          ▼
┌─────────────────┐
│   Return 1      │
│   (hit found)   │
└─────────────────┘
```

## Low-Level Intersection Functions (e.g., intersect_sphere)

```
┌─────────────────────────────────┐
│     intersect_sphere            │
│  (sphere, ray, hit)             │
└─────────────────┬───────────────┘
                  │
                  ▼
┌─────────────────────────────────┐
│   Calculate Quadratic           │
│   Coefficients (a, b, c)       │
│   q = sphere_quadratic_coeffs() │
└─────────────────┬───────────────┘
                  │
                  ▼
┌─────────────────────────────────┐
│     Solve Quadratic             │
│  t = solve_quadratic(q.a,       │
│                     q.b,        │
│                     q.c, 0.001) │
└─────────────────┬───────────────┘
                  │
                  ▼
┌─────────────────────────────────┐
│    Valid Intersection?          │
│    t > 0.001                    │
└─────────────┬───────────────────┘
              │
     ┌────────┴────────┐
     │ YES             │ NO
     ▼                 ▼
┌─────────────────┐  ┌─────────────────┐
│ Closer Hit?     │  │   Return 0      │
│ (hit->t < 0 ||  │  │  (no valid hit) │
│  t < hit->t)    │  └─────────────────┘
└─────┬───────────┘
      │
 ┌────┴────┐
 │ YES     │ NO
 ▼         ▼
┌────────────────┐  ┌─────────────────┐
│ Update Hit:    │  │   Return 0      │
│ hit->t = t     │  │ (farther hit)   │
│ hit->point     │  └─────────────────┘
│ hit->normal    │
│ hit->color     │
│ hit->obj_type  │
│ Return 1       │
└────────────────┘
```

## Data Flow Summary

```
Camera Ray → trace_objects() → Loop through all objects → 
Check object type → Call appropriate trace_[type]() → 
Call intersect_[type]() → Calculate intersection → 
Update closest_hit if closer → Continue loop → 
Return hit_found
```

## Key Variables Tracking

- **i**: Current object index (0 to num_objects-1)
- **hit_found**: Boolean flag (0 or 1) indicating any intersection
- **closest_hit->t**: Distance to closest intersection (-1 if none)
- **closest_hit->obj_index**: Index of closest intersected object
- **Object types**: SPHERE(1), PLANE(2), CYLINDER(3), CONE(4)
