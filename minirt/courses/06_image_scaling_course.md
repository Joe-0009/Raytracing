# Course 6: Image Scaling and Responsive Rendering

## 1. Introduction

Your miniRT must handle window resizing gracefully, adjusting the rendered image to fit the new window size.

---

## 2. Why Image Scaling Matters

- Users may resize the window at any time.
- The rendered image must always fill the window, preserving proportions and quality.

---

## 3. Implementation Approaches

### A. Re-render on Resize (Recommended)

- Listen for window resize events (MiniLibX may provide a callback).
- When the window size changes:
    1. Update internal width/height.
    2. Reallocate image buffer if necessary.
    3. Recompute camera parameters (aspect ratio, FOV).
    4. Raytrace the scene again for the new size.
    5. Display the new image.

### B. Simple Scaling (Not recommended for raytracing)

- Scale the image buffer to fit the new window.
- Fast, but results in blurriness or artifacts.

---

## 4. Handling Aspect Ratio

- Always keep the camera's aspect ratio in sync with the window.
- Adjust camera calculations if the window is not square.

---

## 5. Implementation Example

```c
int handle_resize(int new_width, int new_height, t_scene *scene) {
    scene->width = new_width;
    scene->height = new_height;
    // Free and recreate image buffer
    // Recompute camera aspect ratio
    render(scene); // Raytrace for new size
    mlx_put_image_to_window(...);
    return 0;
}
```

---

## 6. Testing

- Resize the window to various shapes and sizes.
- Check that objects retain their proportions and the image is always sharp.
- Stress test with rapid resizing.

---

## 7. Conclusion

Responsive image scaling ensures a professional and user-friendly experience, fulfilling miniRT’s requirements.