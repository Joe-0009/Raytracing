# Texture Rotation Implementation Guide

## Overview
This implementation adds texture rotation capability to your raytracer, allowing you to rotate textures and bump maps on objects. The rotation is applied in UV space around the center point (0.5, 0.5).

## Key Features
- **UV-based rotation**: Textures are rotated in UV coordinate space
- **Angle support**: Both radians and degrees are supported
- **Unified system**: Works for both textures and bump maps
- **Performance optimized**: Rotation calculations are done per-pixel during sampling

## Implementation Details

### Data Structure
The `t_surface_map` structure now includes:
```c
typedef struct s_surface_map
{
    // ... existing fields ...
    t_uv rotation_uv;  // rotation_uv.u = rotation angle in radians
}
```

### Core Functions

#### 1. Setting Rotation
```c
// Set rotation in radians
void set_texture_rotation(t_surface_map *map, double angle_radians);

// Set rotation in degrees (convenience function)
void set_texture_rotation_degrees(t_surface_map *map, double angle_degrees);
```

#### 2. Internal Rotation Logic
- `rotate_uv()`: Applies 2D rotation matrix to UV coordinates
- `apply_texture_rotation()`: Applies rotation if enabled
- Modified sampling functions: `sample_texture()` and `sample_bump_map()`

## Usage Examples

### Basic Usage
```c
// After loading textures, set rotation
set_texture_rotation_degrees(&sphere.texture, 45.0);  // 45 degrees
set_texture_rotation(&sphere.bump, M_PI / 4);         // π/4 radians
```

### Animation Example
```c
// In your main loop
double time = get_current_time();
double rotation_angle = time * 30.0; // 30 degrees per second
set_texture_rotation_degrees(&sphere.texture, rotation_angle);
```

### Interactive Control
```c
// In your key handler
switch (key) {
    case LEFT_ARROW:
        current_rotation -= 15.0;
        set_texture_rotation_degrees(&texture, current_rotation);
        break;
    case RIGHT_ARROW:
        current_rotation += 15.0;
        set_texture_rotation_degrees(&texture, current_rotation);
        break;
}
```

## Scene File Format Extension (Optional)
You could extend your scene file format to include rotation:
```
sp 0,0,-5 2.0 255,0,0 txm:earth.xpm rot:45
```

## Performance Considerations
- **Rotation calculation**: Done once per pixel sample
- **Memory overhead**: Minimal (just storing rotation angle)
- **Optimization**: Rotation is skipped if angle is near zero

## Integration Steps

1. **Include the new functions**: The texture rotation functions are now available
2. **Initialize rotation**: Rotation values are initialized to 0.0 by default
3. **Set rotation values**: Use `set_texture_rotation_degrees()` or `set_texture_rotation()`
4. **Render**: Textures will automatically be rotated during sampling

## Advanced Usage

### Different Rotation Patterns
```c
// Rotate different objects differently
for (int i = 0; i < scene->num_objects; i++) {
    if (scene->objects[i].type == SPHERE) {
        double angle = i * 90.0; // 0°, 90°, 180°, 270°
        set_texture_rotation_degrees(&scene->objects[i].data.sphere.texture, angle);
    }
}
```

### Synchronized Rotation
```c
// Rotate texture and bump map together
double angle = 45.0;
set_texture_rotation_degrees(&sphere.texture, angle);
set_texture_rotation_degrees(&sphere.bump, angle);
```

### Counter-rotation
```c
// Rotate texture and bump map in opposite directions
set_texture_rotation_degrees(&sphere.texture, angle);
set_texture_rotation_degrees(&sphere.bump, -angle);
```

## Testing Your Implementation

1. **Static test**: Set a 45-degree rotation and verify the texture appears rotated
2. **Animation test**: Continuously rotate texture and verify smooth animation
3. **Multiple objects**: Test different rotation angles on multiple spheres
4. **Bump map test**: Verify bump map rotation works independently

## Troubleshooting

### Common Issues
- **Texture appears flipped**: Check UV mapping orientation
- **Rotation center is wrong**: Ensure center point is (0.5, 0.5)
- **Performance issues**: Verify rotation is only calculated when needed

### Debug Tips
- Print rotation angles to verify they're being set correctly
- Test with high-contrast textures to see rotation clearly
- Use simple geometric patterns for testing

## Future Enhancements

1. **Non-uniform scaling**: Extend rotation_uv to support scaling
2. **Rotation center**: Allow custom rotation centers
3. **3D rotation**: Extend to full 3D texture rotation
4. **Animation curves**: Add easing functions for smooth animation
