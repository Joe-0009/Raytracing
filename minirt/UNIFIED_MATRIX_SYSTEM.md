# Unified Matrix-Based Transformation System

## Overview

This system has been converted from a hybrid vector/matrix approach to a **unified matrix-based transformation system**. All transformations now use 4x4 matrices for consistency, performance, and flexibility.

## Architecture

### Core Components

1. **Matrix Operations** (`matrix_op*.c`)
   - Basic 4x4 matrix operations (multiply, identity)
   - Elementary transformations (translate, rotate X/Y/Z, scale)

2. **Advanced Matrix Operations** (`matrix_advanced.c`)
   - Arbitrary axis rotation using Rodrigues' formula
   - Combined transformations
   - Matrix decomposition
   - Look-at matrix for cameras

3. **Unified Transform System** (`unified_transforms.c`)
   - High-level transformation functions
   - Object transformation utilities
   - Transform composition

4. **Transform Structure**
   ```c
   typedef struct s_transform
   {
       t_vec3    translation;  // Translation vector
       t_vec3    rotation;     // Euler angles (X, Y, Z)
       t_vec3    scale;        // Scale factors
       t_matrix4 matrix;       // Combined transformation matrix
   } t_transform;
   ```

## Key Functions

### Basic Matrix Operations
```c
t_matrix4 matrix4_identity(void);
t_matrix4 matrix4_multiply(t_matrix4 a, t_matrix4 b);
t_matrix4 matrix4_translation(t_vec3 translation);
t_matrix4 matrix4_rotation_x/y/z(double angle);
t_matrix4 matrix4_scale(t_vec3 scale);
```

### Advanced Matrix Operations
```c
t_matrix4 matrix4_rotation_axis(t_vec3 axis, double angle);
t_matrix4 matrix4_transform(t_vec3 translation, t_vec3 rotation, t_vec3 scale);
t_matrix4 matrix4_look_at(t_vec3 eye, t_vec3 target, t_vec3 up);
```

### Unified Transformation API
```c
// Create transforms
t_transform create_unified_transform(t_vec3 translation, t_vec3 rotation, t_vec3 scale);
t_transform create_rotation_transform(t_vec3 rotation);
t_transform create_scale_transform(t_vec3 scale);
t_transform create_translation_transform(t_vec3 translation);

// Apply transforms
void apply_transform_to_object(t_object *object, t_transform *transform);
void scene_transform_object(t_scene *scene, int obj_index,
    t_vec3 translation, t_vec3 rotation, t_vec3 scale);

// Compose multiple transforms
t_transform compose_transforms(t_transform *transforms, int count);
```

### Scene-Level Operations
```c
void scene_translate_object(t_scene *scene, int obj_index, t_vec3 delta);
void scene_rotate_object(t_scene *scene, int obj_index, t_vec3 rotation);
void scene_scale_object(t_scene *scene, int obj_index, double scale);
```

## Migration from Vector-Based System

### Before (Hybrid Approach)
```c
// Direct vector manipulation for rotations
scene->objects[i].data.plane.normal = vec3_rotate_around_axis(
    scene->objects[i].data.plane.normal, axis, angle);

// Mixed matrix/vector operations
sphere.center = matrix4_transform_point(transform.matrix, sphere.center);
```

### After (Unified Matrix System)
```c
// Everything uses matrix transformations
t_transform transform = create_rotation_transform(rotation);
apply_transform_to_object(&scene->objects[i], &transform);

// Or combined operations
scene_transform_object(scene, i, translation, rotation, scale);
```

## Benefits

1. **Consistency**: All transformations use the same mathematical foundation
2. **Performance**: Matrix operations can be optimized and cached
3. **Composability**: Easy to combine multiple transformations
4. **Flexibility**: Support for arbitrary axis rotations
5. **Industry Standard**: Follows common graphics programming practices

## Usage Examples

### Simple Object Rotation
```c
t_vec3 rotation = vec3_create(0.1, 0, 0);  // Rotate around X-axis
scene_rotate_object(scene, object_index, rotation);
```

### Complex Transformation
```c
t_vec3 translation = vec3_create(5, 0, 0);
t_vec3 rotation = vec3_create(0, M_PI/4, 0);
t_vec3 scale = vec3_create(2, 2, 2);

scene_transform_object(scene, object_index, translation, rotation, scale);
```

### Arbitrary Axis Rotation
```c
t_vec3 axis = vec3_normalize(vec3_create(1, 1, 0));
t_vec3 vector = vec3_create(1, 0, 0);
t_vec3 rotated = matrix_rotate_vector(vector, axis, M_PI/3);
```

## Performance Notes

- Matrix operations are optimized for typical transformation scenarios
- Transform matrices are cached and only updated when needed
- Rodrigues' rotation formula provides exact arbitrary axis rotations
- Combined transformations avoid redundant matrix multiplications

## Backwards Compatibility

The system maintains API compatibility with existing code while providing the new unified approach. Old functions like `vec3_rotate_around_axis` are still available but internally use matrix operations where possible.
