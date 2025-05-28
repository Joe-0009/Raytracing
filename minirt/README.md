# miniRT - Raytracer Project

A raytracer implementation in C using the MiniLibX graphics library.

## Project Overview

miniRT is a 3D raytracer that renders scenes containing spheres, planes, and cylinders with ambient lighting and point light sources. The project parses scene files in a custom format and renders them using ray tracing algorithms.

## Features

### Supported Objects
- **Spheres**: Defined by center position, diameter, and color
- **Planes**: Defined by point, normal vector, and color  
- **Cylinders**: Defined by center, axis, diameter, height, and color

### Lighting
- **Ambient Lighting**: Global illumination with adjustable intensity
- **Point Light Sources**: Positioned lights with brightness and color

### Camera
- Configurable position, orientation, and field of view
- Ray casting from camera through each pixel

## Scene File Format

Scene files use the `.rt` extension and follow this format:

```
A  0.2                    255,255,255     # Ambient lighting
L  -40,0,30               0.7             255,255,255     # Light source
C  0,0,-20                0,0,1           70              # Camera

sp 0,0,10                 8               255,0,0         # Sphere
pl 0,-10,0                0,1,0           0,255,0         # Plane
cy 0,0,10                 0,1,0   8   10  0,0,255        # Cylinder
```

### Format Details
- **A**: Ambient lighting (ratio, color)
- **L**: Light source (position, brightness, color)
- **C**: Camera (position, orientation, fov)
- **sp**: Sphere (center, diameter, color)
- **pl**: Plane (point, normal, color)
- **cy**: Cylinder (center, axis, diameter, height, color)

## Build Instructions

```bash
# Compile the project
make

# Clean object files
make clean

# Clean everything including executable
make fclean

# Rebuild everything
make re
```

## Usage

```bash
./miniRT scene_file.rt
```

## Test Scenes

The project includes several test scenes in the `scenes/` directory:

### Basic Scenes
- `simple_test.rt` - Basic scene with one of each object type
- `test_simple.rt` - Simple scene for quick testing
- `test_colors.rt` - Testing different color combinations

### Sphere Collections
Located in `scenes/sphere_scenes/`:
- `test_simple_spheres.rt` - 4 spheres for quick testing
- `test_multiple_spheres.rt` - 9 spheres with varied positioning
- `test_sphere_grid.rt` - 14 spheres in grid formation
- `test_solar_system.rt` - 24 spheres in solar system pattern

### Advanced Scenes
- `test_comprehensive.rt` - Complex scene with multiple objects
- `test_performance.rt` - Performance testing scene
- `test_with_lights.rt` - Testing multiple lighting scenarios

## Project Structure

```
minirt/
├── src/                 # Source code
│   ├── main.c          # Main program entry
│   ├── parser/         # Scene file parsing
│   ├── render/         # Ray tracing and rendering
│   ├── events/         # Event handling
│   └── utils/          # Utility functions
├── includes/           # Header files
├── libft/             # Custom C library
├── scenes/            # Test scene files
├── tests/             # Testing framework
└── Makefile           # Build configuration
```

## Implementation Details

### Parser
- Robust scene file parsing with error handling
- Support for comments and flexible whitespace
- Validation of all numeric values and ranges
- Memory-safe string processing

### Rendering Engine
- Ray-sphere intersection calculations
- Ray-plane intersection calculations  
- Ray-cylinder intersection calculations
- Phong lighting model implementation
- Anti-aliasing support

### Memory Management
- Proper initialization of all data structures
- Safe memory allocation and deallocation
- No memory leaks in normal operation

## Controls

- **ESC**: Exit the program
- **X**: Close window and exit

## Error Handling

The program provides detailed error messages for:
- Invalid command line arguments
- File access errors
- Scene parsing errors
- Invalid object parameters
- Memory allocation failures

## Dependencies

- MiniLibX graphics library
- Standard C library
- Math library (-lm)

## Resolution History

This project has undergone extensive debugging and optimization:

1. **Memory Corruption Issues**: Resolved critical memory corruption affecting light parsing
2. **Object Addition Problems**: Fixed issues where objects weren't being added to scenes
3. **Parser Stability**: Enhanced parser robustness and error handling
4. **Code Organization**: Cleaned up debug code and improved project structure

## Testing

Run the test suite:
```bash
cd tests
./run_all_tests.sh
```

Individual test categories:
```bash
./test_parser.sh        # Parser functionality tests
./test_error_handling.sh # Error handling tests
```

---

**Last Updated**: May 28, 2025  
**Status**: Production Ready ✅
