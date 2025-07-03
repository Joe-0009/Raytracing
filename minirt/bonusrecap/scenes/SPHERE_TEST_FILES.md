# Multiple Spheres Test Files

This directory contains several test scene files specifically designed to test multiple sphere parsing and rendering in the miniRT raytracer.

## Organization

All sphere test files have been moved to `scenes/sphere_scenes/` for better organization:

```
scenes/
├── sphere_scenes/
│   ├── README.md
│   ├── test_simple_spheres.rt      (4 spheres)
│   ├── test_multiple_spheres.rt    (9 spheres)
│   ├── test_sphere_grid.rt         (14 spheres)
│   └── test_solar_system.rt        (24 spheres)
└── [other scene files...]
```

## Test Files Created

### 1. `test_multiple_spheres.rt`
- **Description**: A comprehensive test with 9 spheres of varying sizes and colors
- **Features**: 
  - Large red sphere in center
  - Medium and small spheres arranged around it
  - Different colors: red, blue, green, yellow, purple, orange, white, cyan, pink
  - Various positions in 3D space (front, back, above, below)
- **Use case**: General testing of multiple object parsing and rendering

### 2. `test_sphere_grid.rt`
- **Description**: 14 spheres arranged in a 3x3 grid formation with additional floating spheres
- **Features**:
  - Grid pattern for systematic placement testing
  - Color-coded rows (RGB, YMC, custom colors)
  - Floating spheres at different Z levels
  - Small accent spheres for detail testing
- **Use case**: Testing systematic object placement and camera positioning

### 3. `test_solar_system.rt`
- **Description**: 24 spheres arranged in a solar system pattern
- **Features**:
  - Central large "sun" sphere
  - Inner, middle, and outer "orbital" rings
  - Asteroid belt simulation with tiny spheres
  - Distant objects
  - Light source at center simulating sun
- **Use case**: Complex scene testing with many objects and artistic arrangement

### 4. `test_simple_spheres.rt`
- **Description**: Simple test with just 4 spheres
- **Features**:
  - Three spheres in a row (RGB colors)
  - One sphere in front (yellow)
  - Quick parsing test
- **Use case**: Fast testing and debugging

## Common Features

All test files include:
- ✅ Proper ambient lighting configuration
- ✅ Camera positioning optimized for viewing all spheres
- ✅ Light source placement for good illumination
- ✅ Variety of sphere sizes (2-20 diameter range)
- ✅ Diverse color palette
- ✅ Valid miniRT scene format

## Usage

```bash
# Test individual files
./miniRT test_multiple_spheres.rt
./miniRT test_sphere_grid.rt
./miniRT test_solar_system.rt
./miniRT test_simple_spheres.rt
```

## Expected Output

Each file should parse successfully and display:
- Scene information with correct ambient, light, and camera settings
- All spheres properly parsed with correct positions, sizes, and colors
- No parsing errors
- Successful object count matching the number of spheres in the file

## Testing Results

All test files have been verified to parse correctly with the current miniRT implementation:
- ✅ `test_multiple_spheres.rt` - 9 objects parsed successfully
- ✅ `test_sphere_grid.rt` - 14 objects parsed successfully  
- ✅ `test_solar_system.rt` - 24 objects parsed successfully
- ✅ `test_simple_spheres.rt` - 4 objects parsed successfully

These files provide comprehensive testing coverage for multiple sphere scenarios in the miniRT raytracer.
