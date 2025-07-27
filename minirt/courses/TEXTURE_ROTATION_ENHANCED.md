# Enhanced Texture Rotation System

## Overview
The texture rotation system has been enhanced with improved UV control and more sophisticated rotation handling.

## Features

### 1. Dual-Axis Rotation
- **U-axis rotation**: Rotates texture around the center (0.5, 0.5)
- **V-axis rotation**: Translates texture vertically for additional control

### 2. Improved Functions

#### Core Functions
- `apply_texture_rotation()` - Now supports both U and V rotation
- `set_texture_rotation_uv_degrees()` - Set both U and V rotation in degrees
- `set_sphere_texture_rotation_uv()` - Apply UV rotation to sphere textures

#### Scene Functions
- `scene_rotate_object_texture()` - Enhanced texture rotation with separate U/V control
- `scene_rotate_object()` - Improved logic using actual rotation values

### 3. Key Controls

#### Object Rotation (R, T, F, G)
- **R**: Rotate object +X (or texture +U for spheres)
- **T**: Rotate object +Y 
- **F**: Rotate object -X (or texture -U for spheres)
- **G**: Rotate object -Y

#### Enhanced Texture Rotation (U, I, J, K)
- **U**: Rotate texture +U axis (15°)
- **I**: Rotate texture -U axis (-15°)
- **J**: Rotate texture +V axis (15°)
- **K**: Rotate texture -V axis (-15°)

## Usage Examples

### Basic Texture Rotation
```c
// Set U rotation to 45 degrees, V rotation to 0
set_sphere_texture_rotation_uv(&sphere, 45.0, 0.0);

// Rotate texture incrementally
scene_rotate_object_texture(scene, obj_index, 15.0, 0.0);  // U+
scene_rotate_object_texture(scene, obj_index, 0.0, 15.0);  // V+
```

### Combined Rotation
```c
// Complex texture positioning
set_sphere_texture_rotation_uv(&sphere, 45.0, 30.0);
```

## Implementation Details

### UV Rotation Math
- U rotation uses 2D rotation matrix around center (0.5, 0.5)
- V rotation adds offset and wraps using modulo
- Both support negative values and proper wrapping

### Performance
- Rotation is only applied when angle > 1e-6
- UV coordinates are properly normalized and clamped
- Functions handle edge cases (null pointers, invalid ranges)

## Benefits
1. **More Control**: Separate U and V axis manipulation
2. **Better Logic**: Uses actual rotation values instead of sign checking
3. **Consistent API**: Unified function naming and parameter handling
4. **Backward Compatible**: Original functions still work
5. **Interactive**: Enhanced key bindings for real-time texture manipulation

## Future Enhancements
- Support for texture scaling
- Non-uniform U/V rotation speeds
- Texture animation presets
- Support for other primitive types (planes, cylinders)
