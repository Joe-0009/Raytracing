# Sphere Scenes Collection

This directory contains specialized scene files focused on testing multiple sphere rendering in the miniRT raytracer.

## Scene Files

### 🔴 `test_simple_spheres.rt`
**Quick Test - 4 Spheres**
- Three RGB spheres in a row + one yellow sphere in front
- Perfect for quick testing and debugging
- Minimal complexity for fast parsing verification

**Usage:**
```bash
./miniRT scenes/sphere_scenes/test_simple_spheres.rt
```

### 🌈 `test_multiple_spheres.rt`
**Comprehensive Test - 9 Spheres**
- Large central red sphere surrounded by 8 smaller spheres
- Variety of colors: red, blue, green, yellow, purple, orange, white, cyan, pink
- Different positions: front, back, above, below, left, right
- Good for testing general multiple object parsing and rendering

**Usage:**
```bash
./miniRT scenes/sphere_scenes/test_multiple_spheres.rt
```

### 📐 `test_sphere_grid.rt`
**Grid Formation - 14 Spheres**
- 3x3 grid of spheres with systematic color coding
- Additional floating spheres at different Z levels
- Small accent spheres for detail testing
- Perfect for testing camera positioning and systematic arrangements

**Features:**
- Front row: Red, Green, Blue
- Middle row: Yellow, Magenta (larger center), Cyan  
- Back row: Orange, Purple, Green-cyan
- Floating spheres and accent details

**Usage:**
```bash
./miniRT scenes/sphere_scenes/test_sphere_grid.rt
```

### 🪐 `test_solar_system.rt`
**Solar System Pattern - 24 Spheres**
- Most complex sphere scene with solar system-inspired layout
- Central large "sun" sphere with orbital rings of "planets"
- Asteroid belt simulation with tiny gray spheres
- Distant objects for depth testing

**Features:**
- Central sun (large yellow sphere)
- Inner orbit: 4 small planets
- Middle orbit: 4 medium planets  
- Outer orbit: 4 larger planets
- Asteroid belt: 8 tiny spheres
- Distant objects: 3 far spheres

**Usage:**
```bash
./miniRT scenes/sphere_scenes/test_solar_system.rt
```

## Testing Progression

We recommend testing in this order:

1. **Start Simple**: `test_simple_spheres.rt` (4 spheres)
2. **Add Complexity**: `test_multiple_spheres.rt` (9 spheres)
3. **Test Structure**: `test_sphere_grid.rt` (14 spheres)
4. **Stress Test**: `test_solar_system.rt` (24 spheres)

## Common Settings

All scenes are configured with:
- ✅ Optimized camera positioning for viewing all spheres
- ✅ Proper ambient lighting (10-20% intensity)
- ✅ Well-positioned light sources
- ✅ Diverse color palettes
- ✅ Valid miniRT scene format

## Expected Results

Each scene should:
- Parse without errors
- Display correct object counts
- Show proper sphere positioning and colors
- Render successfully (if rendering is enabled)

## Notes

- All scenes have been tested and verified to parse correctly
- Sphere colors use both RGB primaries and custom color combinations
- Camera angles are optimized for best viewing of all objects
- Light positioning provides good illumination of all spheres

---

**Last Updated**: May 27, 2025  
**Compatible with**: miniRT raytracer implementation
