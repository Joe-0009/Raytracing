# Generating Camera Rays with Ray-Tracing – Summary

## 1. What is a Ray and How is it Used in Ray Tracing?

- A **ray** is mathematically defined by an origin point and a (normalized) direction vector.
- In ray tracing, **primary (camera/eye) rays** are cast from the camera origin through each pixel on the image plane into the scene to compute visibility (i.e., which object is visible for each pixel).
- The intersection with objects is determined by testing each ray against scene geometry. The closest intersection (smallest positive t, where `P = O + t*D`) gives the visible surface for that pixel.
- Rays can be represented in code as a class/struct holding origin, direction, and possibly tMin/tMax for valid intersection ranges.

## 2. Generating Camera Rays

- The camera is, by convention, placed at the world origin `(0, 0, 0)` and looks down the negative z-axis; the image plane is placed at `z = -1`.
- For each pixel, a ray is generated from the camera origin through the center of the corresponding pixel on the image plane.
- Pixel coordinates are transformed from raster space (`[0, width] x [0, height]`) to:
  - **NDC (Normalized Device Coordinates):** `[0, 1]`
  - **Screen space:** `[-aspect, aspect] x [-1, 1]`
- **Aspect ratio** and **field of view** (FOV) are accounted for to ensure correct ray directions and image proportions.
- The direction vector for each ray is constructed as `Vec3f(Px, Py, -1)`, normalized, where Px and Py are the screen-space coordinates.
- The camera-to-world transformation matrix allows for arbitrary camera position and orientation.

## 3. Standard Coordinate Systems (in Ray Tracing vs. Rasterization)

- **Raster space:** Pixel coordinates (origin at top-left, units = pixels)
- **NDC:** For ray tracing, `[0, 1]` in both axes; for OpenGL, `[-1, 1]`
- **Screen space:** After remapping from NDC, covers the visible area in world/camera space
- **World space:** Global coordinate system for all scene objects/cameras
- **Camera space:** Local camera coordinates (aligned to world space when at default position)
- **Object space:** Model's local coordinates before transformation
- **Clipping/window coordinates:** Used in rasterization APIs for visibility tests and final screen mapping

**Warning:** The names "NDC" and "screen space" have different definitions and usage in ray tracing and in APIs like OpenGL.

## 4. High-Level Steps to Generate Camera Rays

1. **Initialize camera parameters:** position, orientation, FOV, image width/height.
2. **For each pixel (i, j):**
   - Convert `(i, j)` to NDC:  
     `u = (i + 0.5) / width`  
     `v = (j + 0.5) / height`
   - Convert to screen space:  
     `x = (2u - 1) * aspect * scale`  
     `y = (1 - 2v) * scale`  
     where `scale = tan(FOV / 2)`
   - Build direction vector:  
     `dir = normalize(Vec3f(x, y, -1))`
   - (If camera is transformed) Apply camera-to-world matrix to origin and direction.
   - Cast ray:  
     `color = castRay(rayOrigin, rayDirection, ...)`
   - Store color in framebuffer.
3. **After processing all pixels:** save framebuffer as an image.

## 5. Reference Implementation Pseudocode

```cpp
float scale = tan(deg2rad(fov * 0.5));
float aspect = width / float(height);

for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
        float x = (2 * (i + 0.5) / width - 1) * aspect * scale;
        float y = (1 - 2 * (j + 0.5) / height) * scale;
        Vec3f dir = normalize(Vec3f(x, y, -1));
        // If using camera-to-world matrix:
        // cameraToWorld.multDirMatrix(dir, dir);
        framebuffer[j * width + i] = castRay(cameraOrigin, dir, ...);
    }
}
```

## 6. Key takeaways

- Generating correct camera rays is foundational for ray tracing.
- Pixel-to-ray transformation requires careful mapping across coordinate spaces and attention to FOV/aspect ratio.
- The method generalizes to arbitrary camera positions via a transformation matrix.
- Different graphics APIs and methods use similar terms but with different meanings; be clear about context.

---
**Next steps:** Learn about ray-object intersection methods to complete the rendering process.