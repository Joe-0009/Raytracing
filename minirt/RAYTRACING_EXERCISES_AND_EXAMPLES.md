# Raytracing Exercises and Practical Examples

This document provides hands-on exercises and examples to help you understand and master raytracing concepts. Each exercise builds upon previous knowledge and includes both theoretical explanation and practical implementation.

## Table of Contents
1. [Getting Started Exercises](#getting-started-exercises)
2. [Vector Mathematics Practice](#vector-mathematics-practice)
3. [Camera and Ray Generation](#camera-and-ray-generation)
4. [Intersection Mathematics](#intersection-mathematics)
5. [Lighting Calculations](#lighting-calculations)
6. [Performance Optimization](#performance-optimization)
7. [Debugging Challenges](#debugging-challenges)
8. [Advanced Projects](#advanced-projects)

## Getting Started Exercises

### Exercise 1: Understanding 3D Coordinates
**Goal:** Visualize 3D coordinate system and understand point representation.

**Problem:** Given these points, describe where they are in 3D space:
- A = (0, 0, 0)
- B = (1, 0, 0)  
- C = (0, 1, 0)
- D = (0, 0, 1)
- E = (1, 1, 1)
- F = (-1, 2, -0.5)

**Solution:**
```
A = Origin (center of coordinate system)
B = 1 unit to the right of origin
C = 1 unit above origin  
D = 1 unit in front of origin (positive Z)
E = Corner of unit cube (right, up, forward)
F = Left of origin, above origin, behind origin
```

**Practical Implementation:**
```c
// Visualize points by printing their distances from origin
void print_point_info(t_point3 p, char *name) {
    double distance = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
    printf("Point %s: (%.2f, %.2f, %.2f) - Distance from origin: %.2f\n", 
           name, p.x, p.y, p.z, distance);
}

// Test it
t_point3 points[] = {
    {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, 
    {0, 0, 1}, {1, 1, 1}, {-1, 2, -0.5}
};
char *names[] = {"A", "B", "C", "D", "E", "F"};

for (int i = 0; i < 6; i++) {
    print_point_info(points[i], names[i]);
}
```

### Exercise 2: Basic Vector Operations
**Goal:** Master fundamental vector arithmetic.

**Problem:** Given vectors A = (3, 4, 0) and B = (1, -2, 5), calculate:
1. A + B
2. A - B  
3. 2 × A
4. Length of A
5. A · B (dot product)
6. A × B (cross product)

**Solution:**
```
1. A + B = (3+1, 4+(-2), 0+5) = (4, 2, 5)
2. A - B = (3-1, 4-(-2), 0-5) = (2, 6, -5)
3. 2 × A = (2×3, 2×4, 2×0) = (6, 8, 0)
4. |A| = √(3² + 4² + 0²) = √(9 + 16) = √25 = 5
5. A · B = 3×1 + 4×(-2) + 0×5 = 3 - 8 + 0 = -5
6. A × B = (4×5 - 0×(-2), 0×1 - 3×5, 3×(-2) - 4×1) = (20, -15, -10)
```

**Practical Implementation:**
```c
void test_vector_operations(void) {
    t_vector3 a = {3, 4, 0};
    t_vector3 b = {1, -2, 5};
    
    printf("Vector A: (%.1f, %.1f, %.1f)\n", a.x, a.y, a.z);
    printf("Vector B: (%.1f, %.1f, %.1f)\n", b.x, b.y, b.z);
    
    t_vector3 sum = vector3_add(a, b);
    printf("A + B = (%.1f, %.1f, %.1f)\n", sum.x, sum.y, sum.z);
    
    t_vector3 diff = vector3_subtract(a, b);
    printf("A - B = (%.1f, %.1f, %.1f)\n", diff.x, diff.y, diff.z);
    
    t_vector3 scaled = vector3_scale(a, 2.0);
    printf("2 × A = (%.1f, %.1f, %.1f)\n", scaled.x, scaled.y, scaled.z);
    
    double length = vector3_length(a);
    printf("|A| = %.1f\n", length);
    
    double dot = vector3_dot(a, b);
    printf("A · B = %.1f\n", dot);
    
    t_vector3 cross = vector3_cross(a, b);
    printf("A × B = (%.1f, %.1f, %.1f)\n", cross.x, cross.y, cross.z);
}
```

## Vector Mathematics Practice

### Exercise 3: Understanding Dot Product Geometry
**Goal:** Understand what dot product tells us about angle between vectors.

**Problem:** Calculate the angle between these vector pairs:
1. A = (1, 0, 0) and B = (0, 1, 0)
2. A = (1, 1, 0) and B = (1, -1, 0)  
3. A = (2, 0, 0) and B = (-3, 0, 0)

**Theory:**
```
cos(θ) = (A · B) / (|A| × |B|)
θ = arccos((A · B) / (|A| × |B|))
```

**Solution:**
```
1. A · B = 1×0 + 0×1 + 0×0 = 0
   |A| = 1, |B| = 1
   cos(θ) = 0/(1×1) = 0 → θ = 90° (perpendicular)

2. A · B = 1×1 + 1×(-1) + 0×0 = 1 - 1 = 0  
   |A| = √2, |B| = √2
   cos(θ) = 0/(√2×√2) = 0 → θ = 90° (perpendicular)

3. A · B = 2×(-3) + 0×0 + 0×0 = -6
   |A| = 2, |B| = 3  
   cos(θ) = -6/(2×3) = -1 → θ = 180° (opposite directions)
```

**Practical Implementation:**
```c
double vector_angle_degrees(t_vector3 a, t_vector3 b) {
    double dot = vector3_dot(a, b);
    double len_a = vector3_length(a);
    double len_b = vector3_length(b);
    
    double cos_theta = dot / (len_a * len_b);
    cos_theta = fmax(-1.0, fmin(1.0, cos_theta)); // Clamp for numerical stability
    
    double theta_radians = acos(cos_theta);
    return theta_radians * 180.0 / M_PI; // Convert to degrees
}

void test_vector_angles(void) {
    t_vector3 pairs[][2] = {
        {{1, 0, 0}, {0, 1, 0}},
        {{1, 1, 0}, {1, -1, 0}},
        {{2, 0, 0}, {-3, 0, 0}}
    };
    
    for (int i = 0; i < 3; i++) {
        double angle = vector_angle_degrees(pairs[i][0], pairs[i][1]);
        printf("Angle %d: %.1f degrees\n", i+1, angle);
    }
}
```

### Exercise 4: Cross Product and Perpendicular Vectors
**Goal:** Understand how cross product creates perpendicular vectors.

**Problem:** Given A = (1, 0, 0) and B = (0, 1, 0), find vector C that is perpendicular to both.

**Theory:** C = A × B gives a vector perpendicular to both A and B.

**Solution:**
```
C = A × B = (1, 0, 0) × (0, 1, 0)
  = (0×0 - 0×1, 0×0 - 1×0, 1×1 - 0×0)
  = (0, 0, 1)
```

**Verification:** C should be perpendicular to both A and B:
```
C · A = (0, 0, 1) · (1, 0, 0) = 0×1 + 0×0 + 1×0 = 0 ✓
C · B = (0, 0, 1) · (0, 1, 0) = 0×0 + 0×1 + 1×0 = 0 ✓
```

**Practical Implementation:**
```c
void test_cross_product_perpendicularity(void) {
    t_vector3 a = {1, 0, 0};
    t_vector3 b = {0, 1, 0};
    t_vector3 c = vector3_cross(a, b);
    
    printf("A = (%.1f, %.1f, %.1f)\n", a.x, a.y, a.z);
    printf("B = (%.1f, %.1f, %.1f)\n", b.x, b.y, b.z);
    printf("C = A × B = (%.1f, %.1f, %.1f)\n", c.x, c.y, c.z);
    
    double dot_ca = vector3_dot(c, a);
    double dot_cb = vector3_dot(c, b);
    
    printf("C · A = %.6f (should be ~0)\n", dot_ca);
    printf("C · B = %.6f (should be ~0)\n", dot_cb);
    
    if (fabs(dot_ca) < 1e-10 && fabs(dot_cb) < 1e-10) {
        printf("✓ C is perpendicular to both A and B!\n");
    }
}
```

## Camera and Ray Generation

### Exercise 5: Understanding Field of View
**Goal:** Visualize how FOV affects what the camera can see.

**Problem:** For a camera at origin looking along positive Z-axis:
1. If FOV = 90°, how far sideways can we see at distance 10?
2. If FOV = 60°, how far sideways can we see at distance 10?
3. What FOV is needed to see 5 units sideways at distance 10?

**Solution:**
```
1. FOV = 90° → tan(45°) = 1
   At distance 10: sideways_distance = 10 × tan(45°) = 10 × 1 = 10 units

2. FOV = 60° → tan(30°) ≈ 0.577  
   At distance 10: sideways_distance = 10 × 0.577 = 5.77 units

3. Need to see 5 units at distance 10:
   tan(FOV/2) = 5/10 = 0.5
   FOV/2 = arctan(0.5) ≈ 26.57°
   FOV ≈ 53.13°
```

**Practical Implementation:**
```c
void test_fov_calculations(void) {
    double fov_90 = 90.0 * M_PI / 180.0;  // Convert to radians
    double fov_60 = 60.0 * M_PI / 180.0;
    double distance = 10.0;
    
    double sideways_90 = distance * tan(fov_90 / 2.0);
    double sideways_60 = distance * tan(fov_60 / 2.0);
    
    printf("At distance %.1f:\n", distance);
    printf("FOV 90°: can see ±%.2f units sideways\n", sideways_90);
    printf("FOV 60°: can see ±%.2f units sideways\n", sideways_60);
    
    // Reverse calculation
    double target_sideways = 5.0;
    double half_fov_needed = atan(target_sideways / distance);
    double fov_needed = 2.0 * half_fov_needed * 180.0 / M_PI;
    
    printf("To see ±%.1f units at distance %.1f: need FOV %.2f°\n", 
           target_sideways, distance, fov_needed);
}
```

### Exercise 6: Pixel to Ray Conversion
**Goal:** Convert screen pixel coordinates to 3D ray directions.

**Problem:** Screen is 800×600 pixels, FOV = 90°, camera at origin looking along Z-axis. Convert these pixels to rays:
1. Center pixel (400, 300)
2. Top-left corner (0, 0)  
3. Bottom-right corner (800, 600)
4. Arbitrary pixel (600, 200)

**Solution:**
```c
t_ray pixel_to_ray_exercise(int pixel_x, int pixel_y, int width, int height, double fov_degrees) {
    double fov_radians = fov_degrees * M_PI / 180.0;
    double pixel_scale = tan(fov_radians / 2.0) / (width / 2.0);
    
    // Convert to centered coordinates
    double u = (pixel_x - width/2.0) * pixel_scale;
    double v = (height/2.0 - pixel_y) * pixel_scale;  // Flip Y axis
    
    // For camera looking along Z-axis
    t_vector3 direction = {u, v, 1.0};
    direction = vector3_normalize(direction);
    
    t_ray ray = {{0, 0, 0}, direction};  // Camera at origin
    return ray;
}

void test_pixel_to_ray_conversion(void) {
    int width = 800, height = 600;
    double fov = 90.0;
    
    int test_pixels[][2] = {
        {400, 300},  // Center
        {0, 0},      // Top-left
        {800, 600},  // Bottom-right  
        {600, 200}   // Arbitrary
    };
    char *descriptions[] = {"Center", "Top-left", "Bottom-right", "Arbitrary"};
    
    for (int i = 0; i < 4; i++) {
        t_ray ray = pixel_to_ray_exercise(test_pixels[i][0], test_pixels[i][1], 
                                        width, height, fov);
        printf("%s pixel (%d, %d) → Ray direction: (%.3f, %.3f, %.3f)\n",
               descriptions[i], test_pixels[i][0], test_pixels[i][1],
               ray.direction.x, ray.direction.y, ray.direction.z);
    }
}
```

## Intersection Mathematics

### Exercise 7: Ray-Sphere Intersection Step by Step
**Goal:** Manually solve ray-sphere intersection to understand the mathematics.

**Problem:** Ray starts at (-3, 0, 0) with direction (1, 0, 0). Sphere centered at (0, 0, 0) with radius 2. Find intersection points.

**Step-by-step Solution:**
```
1. Ray equation: R(t) = (-3, 0, 0) + t(1, 0, 0) = (-3+t, 0, 0)
2. Sphere equation: x² + y² + z² = 4
3. Substitute ray into sphere: (-3+t)² + 0² + 0² = 4
4. Expand: 9 - 6t + t² = 4
5. Rearrange: t² - 6t + 5 = 0
6. Quadratic formula: t = (6 ± √(36-20))/2 = (6 ± √16)/2 = (6 ± 4)/2
7. Solutions: t₁ = 1, t₂ = 5
8. Intersection points: 
   - At t=1: R(1) = (-2, 0, 0)
   - At t=5: R(5) = (2, 0, 0)
```

**Practical Implementation:**
```c
void solve_ray_sphere_manually(void) {
    // Ray: origin (-3, 0, 0), direction (1, 0, 0)
    t_point3 ray_origin = {-3, 0, 0};
    t_vector3 ray_direction = {1, 0, 0};
    
    // Sphere: center (0, 0, 0), radius 2
    t_point3 sphere_center = {0, 0, 0};
    double sphere_radius = 2.0;
    
    // Calculate quadratic coefficients
    t_vector3 oc = vector3_subtract(ray_origin, sphere_center);
    double a = vector3_dot(ray_direction, ray_direction);  // Should be 1
    double b = 2.0 * vector3_dot(oc, ray_direction);
    double c = vector3_dot(oc, oc) - sphere_radius * sphere_radius;
    
    printf("Quadratic coefficients: a=%.1f, b=%.1f, c=%.1f\n", a, b, c);
    
    double discriminant = b*b - 4*a*c;
    printf("Discriminant: %.1f\n", discriminant);
    
    if (discriminant >= 0) {
        double sqrt_disc = sqrt(discriminant);
        double t1 = (-b - sqrt_disc) / (2*a);
        double t2 = (-b + sqrt_disc) / (2*a);
        
        printf("Intersection distances: t1=%.1f, t2=%.1f\n", t1, t2);
        
        // Calculate intersection points
        t_point3 p1 = vector3_add(ray_origin, vector3_scale(ray_direction, t1));
        t_point3 p2 = vector3_add(ray_origin, vector3_scale(ray_direction, t2));
        
        printf("Intersection points:\n");
        printf("  P1 at t=%.1f: (%.1f, %.1f, %.1f)\n", t1, p1.x, p1.y, p1.z);
        printf("  P2 at t=%.1f: (%.1f, %.1f, %.1f)\n", t2, p2.x, p2.y, p2.z);
    } else {
        printf("No intersection (ray misses sphere)\n");
    }
}
```

### Exercise 8: Understanding Surface Normals
**Goal:** Calculate and visualize surface normals for different shapes.

**Problem:** For these intersection points, calculate the surface normals:
1. Sphere (center=origin, radius=1) hit at (0.6, 0.8, 0)
2. Plane (point=origin, normal=(0,1,0)) hit at (5, 0, 3)
3. Cylinder (axis along Z, radius=1) hit at (0.8, 0.6, 2)

**Solution:**
```
1. Sphere normal = normalize(hit_point - center) = normalize((0.6, 0.8, 0) - (0,0,0)) = (0.6, 0.8, 0)
   (Already normalized since point is on unit sphere)

2. Plane normal = given normal = (0, 1, 0)
   (Constant for entire plane)

3. Cylinder normal = normalize(hit_point_projected_to_XY_plane)
   = normalize((0.8, 0.6, 0)) = (0.8, 0.6, 0)
   (Remove Z component, then normalize)
```

**Practical Implementation:**
```c
void test_surface_normals(void) {
    printf("Testing surface normal calculations:\n\n");
    
    // 1. Sphere normal
    t_point3 sphere_center = {0, 0, 0};
    t_point3 sphere_hit = {0.6, 0.8, 0};
    t_vector3 sphere_normal = vector3_normalize(
        vector3_subtract(sphere_hit, sphere_center)
    );
    printf("Sphere normal at (%.1f, %.1f, %.1f): (%.1f, %.1f, %.1f)\n",
           sphere_hit.x, sphere_hit.y, sphere_hit.z,
           sphere_normal.x, sphere_normal.y, sphere_normal.z);
    
    // 2. Plane normal (constant)
    t_vector3 plane_normal = {0, 1, 0};
    t_point3 plane_hit = {5, 0, 3};
    printf("Plane normal at (%.1f, %.1f, %.1f): (%.1f, %.1f, %.1f)\n",
           plane_hit.x, plane_hit.y, plane_hit.z,
           plane_normal.x, plane_normal.y, plane_normal.z);
    
    // 3. Cylinder normal (remove Z component)
    t_point3 cylinder_hit = {0.8, 0.6, 2};
    t_vector3 cylinder_normal = vector3_normalize((t_vector3){
        cylinder_hit.x, cylinder_hit.y, 0  // Remove Z component
    });
    printf("Cylinder normal at (%.1f, %.1f, %.1f): (%.1f, %.1f, %.1f)\n",
           cylinder_hit.x, cylinder_hit.y, cylinder_hit.z,
           cylinder_normal.x, cylinder_normal.y, cylinder_normal.z);
    
    // Verify normals are unit length
    printf("\nNormal lengths (should be 1.0):\n");
    printf("Sphere: %.6f\n", vector3_length(sphere_normal));
    printf("Plane: %.6f\n", vector3_length(plane_normal));
    printf("Cylinder: %.6f\n", vector3_length(cylinder_normal));
}
```

## Lighting Calculations

### Exercise 9: Diffuse Lighting Step by Step
**Goal:** Calculate diffuse lighting manually using Lambert's law.

**Problem:** 
- Surface point at (2, 1, 0) with normal (0, 1, 0)
- Light at (0, 5, 0) with intensity 0.8 and color (1, 1, 1)
- Surface color (0.8, 0.2, 0.2) [red]
- Calculate final diffuse color.

**Step-by-step Solution:**
```
1. Light direction = normalize(light_pos - surface_pos)
   = normalize((0,5,0) - (2,1,0)) = normalize((-2,4,0))
   = (-2,4,0) / √(4+16) = (-2,4,0) / √20 = (-0.447, 0.894, 0)

2. Light intensity factor = max(0, normal · light_direction)
   = max(0, (0,1,0) · (-0.447,0.894,0)) = max(0, 0.894) = 0.894

3. Distance attenuation = 1 / (1 + kl*d + kq*d²)
   Distance d = √20 ≈ 4.47
   Using kl=0.01, kq=0.001:
   = 1 / (1 + 0.01*4.47 + 0.001*20) = 1 / (1 + 0.0447 + 0.02) = 1/1.0647 ≈ 0.939

4. Final diffuse color = light_color × surface_color × intensity × light_factor × attenuation
   = (1,1,1) × (0.8,0.2,0.2) × 0.8 × 0.894 × 0.939
   = (0.8,0.2,0.2) × 0.672
   = (0.538, 0.134, 0.134)
```

**Practical Implementation:**
```c
void calculate_diffuse_lighting_exercise(void) {
    t_point3 surface_point = {2, 1, 0};
    t_vector3 surface_normal = {0, 1, 0};
    t_color3 surface_color = {0.8, 0.2, 0.2};
    
    t_point3 light_position = {0, 5, 0};
    t_color3 light_color = {1, 1, 1};
    double light_intensity = 0.8;
    
    // Step 1: Light direction
    t_vector3 light_dir = vector3_normalize(
        vector3_subtract(light_position, surface_point)
    );
    printf("Light direction: (%.3f, %.3f, %.3f)\n", 
           light_dir.x, light_dir.y, light_dir.z);
    
    // Step 2: Light intensity factor
    double light_factor = fmax(0.0, vector3_dot(surface_normal, light_dir));
    printf("Light factor (N·L): %.3f\n", light_factor);
    
    // Step 3: Distance attenuation
    double distance = vector3_length(vector3_subtract(light_position, surface_point));
    double attenuation = 1.0 / (1.0 + 0.01*distance + 0.001*distance*distance);
    printf("Distance: %.3f, Attenuation: %.3f\n", distance, attenuation);
    
    // Step 4: Final color
    double total_factor = light_intensity * light_factor * attenuation;
    t_color3 final_color = {
        surface_color.r * light_color.r * total_factor,
        surface_color.g * light_color.g * total_factor,
        surface_color.b * light_color.b * total_factor
    };
    
    printf("Final diffuse color: (%.3f, %.3f, %.3f)\n",
           final_color.r, final_color.g, final_color.b);
}
```

### Exercise 10: Shadow Calculations
**Goal:** Understand shadow ray casting and occlusion testing.

**Problem:** 
- Surface point at (0, 0, 0)
- Light at (5, 5, 5)  
- Potential blocking sphere at (2.5, 2.5, 2.5) with radius 1
- Is the surface point in shadow?

**Solution:**
```
1. Shadow ray: origin=(0,0,0) + ε*light_dir, direction=normalize((5,5,5)-(0,0,0))
   Light direction = (5,5,5) / √75 = (0.577, 0.577, 0.577)
   Shadow ray origin = (0,0,0) + 0.0001*(0.577,0.577,0.577) ≈ (0.0001,0.0001,0.0001)

2. Test intersection with blocking sphere:
   Ray to sphere center: (2.5,2.5,2.5) - (0.0001,0.0001,0.0001) ≈ (2.5,2.5,2.5)
   
3. Ray-sphere intersection test:
   - Distance to sphere center: √(2.5²+2.5²+2.5²) = √18.75 ≈ 4.33
   - If ray hits sphere and hit distance < light distance (√75 ≈ 8.66), then in shadow

4. Solve intersection: Ray hits sphere at distance ≈ 3.33, which is < 8.66
   Therefore: Surface point IS in shadow
```

**Practical Implementation:**
```c
typedef struct {
    t_point3 center;
    double radius;
} t_simple_sphere;

int test_shadow_ray(t_point3 surface_point, t_point3 light_pos, 
                   t_simple_sphere blocking_sphere) {
    // Create shadow ray
    t_vector3 light_dir = vector3_normalize(
        vector3_subtract(light_pos, surface_point)
    );
    t_point3 shadow_origin = vector3_add(surface_point, 
                                        vector3_scale(light_dir, 0.0001));
    
    // Distance to light
    double light_distance = vector3_length(
        vector3_subtract(light_pos, surface_point)
    );
    
    printf("Shadow ray origin: (%.4f, %.4f, %.4f)\n",
           shadow_origin.x, shadow_origin.y, shadow_origin.z);
    printf("Light direction: (%.3f, %.3f, %.3f)\n",
           light_dir.x, light_dir.y, light_dir.z);
    printf("Distance to light: %.3f\n", light_distance);
    
    // Test intersection with blocking sphere
    t_vector3 oc = vector3_subtract(shadow_origin, blocking_sphere.center);
    double a = 1.0;  // light_dir is normalized
    double b = 2.0 * vector3_dot(oc, light_dir);
    double c = vector3_dot(oc, oc) - blocking_sphere.radius * blocking_sphere.radius;
    
    double discriminant = b*b - 4*a*c;
    
    if (discriminant < 0) {
        printf("No intersection with blocking sphere\n");
        return 0;  // Not in shadow
    }
    
    double t = (-b - sqrt(discriminant)) / (2*a);
    if (t > 0 && t < light_distance) {
        printf("Intersection at distance %.3f (< %.3f light distance)\n", t, light_distance);
        return 1;  // In shadow
    }
    
    printf("No blocking intersection\n");
    return 0;  // Not in shadow
}

void test_shadow_calculation(void) {
    t_point3 surface = {0, 0, 0};
    t_point3 light = {5, 5, 5};
    t_simple_sphere blocker = {{2.5, 2.5, 2.5}, 1.0};
    
    int in_shadow = test_shadow_ray(surface, light, blocker);
    printf("Surface point is %s\n", in_shadow ? "IN SHADOW" : "ILLUMINATED");
}
```

## Performance Optimization

### Exercise 11: Early Ray Termination Analysis
**Goal:** Understand when and how early termination improves performance.

**Problem:** Scene has 100 objects. Ray intersects object #3 at distance 0.001. Should we check remaining 97 objects?

**Analysis:**
```c
void analyze_early_termination(void) {
    const double EARLY_TERMINATION_DISTANCE = 0.002;
    
    // Simulation data
    int total_objects = 100;
    int checked_objects = 3;
    double closest_hit_distance = 0.001;
    
    printf("Scene analysis:\n");
    printf("Total objects: %d\n", total_objects);
    printf("Objects checked: %d\n", checked_objects);
    printf("Closest hit distance: %.6f\n", closest_hit_distance);
    printf("Early termination threshold: %.6f\n", EARLY_TERMINATION_DISTANCE);
    
    if (closest_hit_distance < EARLY_TERMINATION_DISTANCE) {
        int objects_skipped = total_objects - checked_objects;
        double time_saved_percent = (double)objects_skipped / total_objects * 100;
        
        printf("\n✓ EARLY TERMINATION TRIGGERED\n");
        printf("Objects skipped: %d\n", objects_skipped);
        printf("Estimated time saved: %.1f%%\n", time_saved_percent);
        printf("Reasoning: Hit is very close (%.6f < %.6f), unlikely to find closer hit\n",
               closest_hit_distance, EARLY_TERMINATION_DISTANCE);
    } else {
        printf("\n✗ Continue checking remaining objects\n");
        printf("Hit distance %.6f >= threshold %.6f\n", 
               closest_hit_distance, EARLY_TERMINATION_DISTANCE);
    }
}
```

### Exercise 12: Discriminant Optimization Impact
**Goal:** Measure performance impact of discriminant checking.

**Problem:** Compare performance with and without discriminant optimization for sphere intersections.

**Implementation:**
```c
#include <time.h>

// Without optimization
int sphere_intersect_slow(t_point3 ray_origin, t_vector3 ray_dir,
                         t_point3 sphere_center, double radius) {
    t_vector3 oc = vector3_subtract(ray_origin, sphere_center);
    double a = vector3_dot(ray_dir, ray_dir);
    double b = 2.0 * vector3_dot(oc, ray_dir);
    double c = vector3_dot(oc, oc) - radius * radius;
    
    double discriminant = b*b - 4*a*c;
    if (discriminant < 0) return 0;
    
    double sqrt_discriminant = sqrt(discriminant);  // Always calculated
    double t1 = (-b - sqrt_discriminant) / (2*a);
    double t2 = (-b + sqrt_discriminant) / (2*a);
    
    return (t1 > 0 || t2 > 0) ? 1 : 0;
}

// With discriminant optimization
int sphere_intersect_fast(t_point3 ray_origin, t_vector3 ray_dir,
                         t_point3 sphere_center, double radius) {
    t_vector3 oc = vector3_subtract(ray_origin, sphere_center);
    double a = vector3_dot(ray_dir, ray_dir);
    double b = 2.0 * vector3_dot(oc, ray_dir);
    double c = vector3_dot(oc, oc) - radius * radius;
    
    // Quick discriminant check
    if (b*b < 4*a*c) return 0;  // Early exit, no sqrt needed
    
    double sqrt_discriminant = sqrt(b*b - 4*a*c);  // Only when needed
    double t1 = (-b - sqrt_discriminant) / (2*a);
    double t2 = (-b + sqrt_discriminant) / (2*a);
    
    return (t1 > 0 || t2 > 0) ? 1 : 0;
}

void benchmark_discriminant_optimization(void) {
    const int NUM_TESTS = 1000000;
    
    // Test data - mix of hits and misses
    t_point3 ray_origins[10];
    t_vector3 ray_dirs[10];
    t_point3 sphere_centers[5];
    double radii[5] = {1.0, 2.0, 0.5, 3.0, 1.5};
    
    // Initialize test data (random rays and spheres)
    for (int i = 0; i < 10; i++) {
        ray_origins[i] = (t_point3){rand()%20-10, rand()%20-10, rand()%20-10};
        ray_dirs[i] = vector3_normalize((t_vector3){
            (double)rand()/RAND_MAX*2-1,
            (double)rand()/RAND_MAX*2-1, 
            (double)rand()/RAND_MAX*2-1
        });
    }
    for (int i = 0; i < 5; i++) {
        sphere_centers[i] = (t_point3){rand()%10-5, rand()%10-5, rand()%10-5};
    }
    
    // Benchmark slow version
    clock_t start = clock();
    int hits_slow = 0;
    for (int test = 0; test < NUM_TESTS; test++) {
        int ray_idx = test % 10;
        int sphere_idx = test % 5;
        hits_slow += sphere_intersect_slow(ray_origins[ray_idx], ray_dirs[ray_idx],
                                          sphere_centers[sphere_idx], radii[sphere_idx]);
    }
    clock_t end_slow = clock();
    
    // Benchmark fast version
    start = clock();
    int hits_fast = 0;
    for (int test = 0; test < NUM_TESTS; test++) {
        int ray_idx = test % 10;
        int sphere_idx = test % 5;
        hits_fast += sphere_intersect_fast(ray_origins[ray_idx], ray_dirs[ray_idx],
                                          sphere_centers[sphere_idx], radii[sphere_idx]);
    }
    clock_t end_fast = clock();
    
    double time_slow = ((double)(end_slow - start)) / CLOCKS_PER_SEC;
    double time_fast = ((double)(end_fast - start)) / CLOCKS_PER_SEC;
    double speedup = time_slow / time_fast;
    
    printf("Discriminant Optimization Benchmark (%d tests):\n", NUM_TESTS);
    printf("Slow version: %.4f seconds (%d hits)\n", time_slow, hits_slow);
    printf("Fast version: %.4f seconds (%d hits)\n", time_fast, hits_fast);
    printf("Speedup: %.2fx (%.1f%% faster)\n", speedup, (speedup-1)*100);
    printf("Results match: %s\n", (hits_slow == hits_fast) ? "YES" : "NO");
}
```

## Debugging Challenges

### Exercise 13: Fixing Common Ray-Sphere Bugs
**Goal:** Identify and fix typical intersection calculation errors.

**Buggy Code:**
```c
// This code has several bugs - find and fix them!
int buggy_sphere_intersection(t_ray ray, t_sphere sphere, t_hit *hit) {
    t_vector3 oc = vector3_subtract(ray.origin, sphere.center);
    double a = vector3_dot(ray.direction, ray.direction);
    double b = vector3_dot(oc, ray.direction);  // BUG 1: Missing factor of 2
    double c = vector3_dot(oc, oc) - sphere.radius;  // BUG 2: Should be radius²
    
    double discriminant = b*b - 4*a*c;
    if (discriminant > 0) {  // BUG 3: Should be >= to handle tangent case
        double t = (-b - sqrt(discriminant)) / (2*a);
        if (t > 0) {  // BUG 4: Should check minimum distance
            hit->t = t;
            hit->point = vector3_add(ray.origin, vector3_scale(ray.direction, t));
            hit->normal = vector3_subtract(hit->point, sphere.center);  // BUG 5: Not normalized
            return 1;
        }
    }
    return 0;
}
```

**Fixed Version:**
```c
int fixed_sphere_intersection(t_ray ray, t_sphere sphere, t_hit *hit) {
    t_vector3 oc = vector3_subtract(ray.origin, sphere.center);
    double a = vector3_dot(ray.direction, ray.direction);
    double b = 2.0 * vector3_dot(oc, ray.direction);  // FIX 1: Added factor of 2
    double c = vector3_dot(oc, oc) - sphere.radius * sphere.radius;  // FIX 2: radius²
    
    double discriminant = b*b - 4*a*c;
    if (discriminant >= 0) {  // FIX 3: >= to handle tangent case
        double t = (-b - sqrt(discriminant)) / (2*a);
        if (t > MIN_T) {  // FIX 4: Check minimum distance constant
            hit->t = t;
            hit->point = vector3_add(ray.origin, vector3_scale(ray.direction, t));
            hit->normal = vector3_normalize(  // FIX 5: Normalize the normal
                vector3_subtract(hit->point, sphere.center)
            );
            return 1;
        }
    }
    return 0;
}

void test_bug_fixes(void) {
    t_ray test_ray = {{-2, 0, 0}, {1, 0, 0}};
    t_sphere test_sphere = {{0, 0, 0}, 1.0, {1, 0, 0}};
    t_hit hit_buggy, hit_fixed;
    
    printf("Testing ray-sphere intersection fixes:\n");
    printf("Ray: origin(-2,0,0), direction(1,0,0)\n");
    printf("Sphere: center(0,0,0), radius=1\n\n");
    
    int result_buggy = buggy_sphere_intersection(test_ray, test_sphere, &hit_buggy);
    int result_fixed = fixed_sphere_intersection(test_ray, test_sphere, &hit_fixed);
    
    printf("Buggy version result: %d\n", result_buggy);
    if (result_buggy) {
        printf("  t=%.3f, point=(%.3f,%.3f,%.3f), normal_length=%.3f\n",
               hit_buggy.t, hit_buggy.point.x, hit_buggy.point.y, hit_buggy.point.z,
               vector3_length(hit_buggy.normal));
    }
    
    printf("Fixed version result: %d\n", result_fixed);
    if (result_fixed) {
        printf("  t=%.3f, point=(%.3f,%.3f,%.3f), normal_length=%.3f\n",
               hit_fixed.t, hit_fixed.point.x, hit_fixed.point.y, hit_fixed.point.z,
               vector3_length(hit_fixed.normal));
    }
}
```

## Advanced Projects

### Project 1: Implement Soft Shadows
**Goal:** Create realistic soft shadows using area lights.

**Theory:** Instead of point lights, use rectangular area lights and sample multiple points on the light surface.

**Implementation Outline:**
```c
typedef struct {
    t_point3 center;
    t_vector3 u_axis, v_axis;  // Rectangle edges
    double width, height;
    t_color3 color;
    double intensity;
} t_area_light;

double calculate_soft_shadow_factor(t_point3 surface_point, t_area_light *light, 
                                   t_scene *scene, int num_samples) {
    int illuminated_samples = 0;
    
    for (int i = 0; i < num_samples; i++) {
        // Random point on light surface
        double u = ((double)rand() / RAND_MAX - 0.5) * light->width;
        double v = ((double)rand() / RAND_MAX - 0.5) * light->height;
        
        t_point3 light_sample = vector3_add(light->center,
            vector3_add(vector3_scale(light->u_axis, u),
                       vector3_scale(light->v_axis, v)));
        
        // Test shadow ray to this sample point
        if (!is_point_in_shadow(surface_point, light_sample, scene)) {
            illuminated_samples++;
        }
    }
    
    return (double)illuminated_samples / num_samples;
}
```

### Project 2: Add Reflection Support
**Goal:** Implement perfect mirror reflections.

**Theory:** When a ray hits a reflective surface, generate a new ray in the reflection direction and trace it recursively.

**Implementation:**
```c
t_color3 trace_ray_with_reflections(t_ray ray, t_scene *scene, int depth) {
    if (depth <= 0) return (t_color3){0, 0, 0};  // Max recursion depth
    
    t_hit hit;
    if (!trace_objects(scene, ray, &hit)) {
        return scene->background_color;
    }
    
    // Calculate basic lighting
    t_color3 local_color = calculate_lighting(scene, &hit);
    
    // If surface is reflective, add reflection contribution
    if (hit.material.reflectivity > 0) {
        t_vector3 reflection_dir = reflect_vector(ray.direction, hit.normal);
        t_ray reflection_ray = {
            vector3_add(hit.point, vector3_scale(hit.normal, EPSILON)),
            reflection_dir
        };
        
        t_color3 reflection_color = trace_ray_with_reflections(reflection_ray, scene, depth - 1);
        
        // Blend local and reflection colors
        local_color = vector3_lerp(local_color, reflection_color, hit.material.reflectivity);
    }
    
    return local_color;
}
```

### Project 3: Implement Depth of Field
**Goal:** Add camera focus effects where objects at different distances appear blurred.

**Theory:** Instead of single rays per pixel, generate multiple rays from slightly different positions on the camera "aperture."

**Implementation:**
```c
typedef struct {
    t_point3 position;
    t_vector3 orientation;
    double fov;
    double aperture_size;    // Size of camera aperture
    double focus_distance;   // Distance to focus plane
} t_camera_dof;

t_color3 render_pixel_with_dof(t_camera_dof *camera, int x, int y, 
                              t_scene *scene, int samples) {
    t_color3 accumulated_color = {0, 0, 0};
    
    for (int i = 0; i < samples; i++) {
        // Random point on aperture (circular)
        double angle = 2.0 * M_PI * rand() / RAND_MAX;
        double radius = camera->aperture_size * sqrt((double)rand() / RAND_MAX);
        t_point3 aperture_offset = {
            radius * cos(angle),
            radius * sin(angle),
            0
        };
        
        // Calculate ray from aperture point through focus plane
        t_ray ray = generate_dof_ray(camera, x, y, aperture_offset);
        t_color3 sample_color = trace_ray(ray, scene);
        
        accumulated_color = vector3_add(accumulated_color, sample_color);
    }
    
    return vector3_scale(accumulated_color, 1.0 / samples);
}
```

This comprehensive set of exercises and examples provides hands-on experience with every aspect of raytracing, from basic mathematics to advanced rendering techniques. Each exercise builds understanding progressively and includes both theoretical explanation and practical implementation.
