# MLX OpenGL Complete Course

## Table of Contents
1. [Introduction](#introduction)
2. [Core System Functions](#core-system-functions)
3. [Window Management](#window-management)
4. [Image Loading & Management](#image-loading--management)
5. [Color & RGB System](#color--rgb-system)
6. [Font & Text System](#font--text-system)
7. [Event Handling](#event-handling)
8. [OpenGL Specific Features](#opengl-specific-features)
9. [Advanced Utilities](#advanced-utilities)
10. [Practical Examples](#practical-examples)
11. [Best Practices](#best-practices)

---

## Introduction

MLX (MiniLibX) is a graphics library that provides a simple interface for creating graphical applications. The OpenGL version extends the basic MLX with hardware acceleration and advanced graphics features.

### Basic MLX vs OpenGL MLX
- **Basic MLX**: Software rendering, basic features
- **OpenGL MLX**: Hardware acceleration, shaders, PNG support, advanced graphics

---

## Core System Functions

### `mlx_init()`
**Purpose**: Initialize the MLX system and create a connection to the graphics server.

**Use Cases**:
- Starting any MLX application
- Setting up graphics context
- Preparing for window creation

**Example**:
```c
void *mlx_ptr = mlx_init();
if (!mlx_ptr)
    exit(1); // Failed to initialize MLX
```

---

## Window Management

### `mlx_new_window(mlx_ptr, width, height, title)`
**Purpose**: Create a new display window.

**Use Cases**:
- Creating main application window
- Setting up rendering canvas
- Displaying raytracer output
- Creating UI dialogs

**Example**:
```c
void *win_ptr = mlx_new_window(mlx_ptr, 800, 600, "My Raytracer");
```

### `mlx_destroy_window(mlx_ptr, win_ptr)`
**Purpose**: Close and free a window.

**Use Cases**:
- Cleaning up on application exit
- Closing dialog windows
- Memory management

**Example**:
```c
mlx_destroy_window(mlx_ptr, win_ptr);
```

### `mlx_clear_window(mlx_ptr, win_ptr)`
**Purpose**: Clear the entire window contents.

**Use Cases**:
- Clearing canvas before redrawing
- Preparing for new frame
- Resetting display

**Example**:
```c
mlx_clear_window(mlx_ptr, win_ptr);
```

### `mlx_pixel_put(mlx_ptr, win_ptr, x, y, color)`
**Purpose**: Draw a single pixel at specified coordinates.

**Use Cases**:
- Drawing individual pixels in raytracer
- Creating simple graphics
- Debug visualization
- Pixel-level drawing

**Example**:
```c
mlx_pixel_put(mlx_ptr, win_ptr, 100, 100, 0xFF0000); // Red pixel
```

### `mlx_screen_size(mlx_ptr, &width, &height)`
**Purpose**: Get the display screen dimensions.

**Use Cases**:
- Adaptive window sizing
- Fullscreen applications
- Screen-relative positioning
- Resolution-dependent rendering

**Example**:
```c
int width, height;
mlx_screen_size(mlx_ptr, &width, &height);
// Create window using screen dimensions
```

---

## Image Loading & Management

### `mlx_xpm_file_to_image(mlx_ptr, filename, &width, &height)`
**Purpose**: Load XPM image file into memory.

**Use Cases**:
- Loading texture maps for 3D objects
- Loading bump maps for surface detail
- Loading UI elements (buttons, icons)
- Loading sprite graphics
- Loading environment maps

**Example**:
```c
int width, height;
void *img = mlx_xpm_file_to_image(mlx_ptr, "texture.xpm", &width, &height);
```

### `mlx_png_file_to_image(mlx_ptr, filename, &width, &height)`
**Purpose**: Load PNG image file into memory (OpenGL version only).

**Use Cases**:
- Loading high-quality textures
- Loading photographs as textures
- Loading modern image formats
- Loading UI graphics with transparency
- Loading HDR environment maps

**Example**:
```c
int width, height;
void *img = mlx_png_file_to_image(mlx_ptr, "texture.png", &width, &height);
```

### `mlx_xpm_to_image(mlx_ptr, xpm_data, &width, &height)`
**Purpose**: Create image from XPM data in memory.

**Use Cases**:
- Embedding images in source code
- Creating images from string data
- Dynamic image generation
- Loading compiled-in graphics

**Example**:
```c
char *xpm_data[] = {"2 2 2 1", "# c #FF0000", ". c #0000FF", "##", ".."};
void *img = mlx_xpm_to_image(mlx_ptr, xpm_data, &width, &height);
```

### `mlx_new_image(mlx_ptr, width, height)`
**Purpose**: Create new empty image buffer.

**Use Cases**:
- Creating render targets
- Creating framebuffers for raytracing
- Creating temporary image buffers
- Dynamic image generation

**Example**:
```c
void *img = mlx_new_image(mlx_ptr, 800, 600);
```

### `mlx_get_data_addr(img_ptr, &bits_per_pixel, &line_length, &endian)`
**Purpose**: Get direct access to image pixel data.

**Use Cases**:
- Direct pixel manipulation for raytracing
- Texture sampling for 3D rendering
- Fast image processing
- Custom drawing operations
- Reading texture colors

**Example**:
```c
int bpp, line_len, endian;
char *data = mlx_get_data_addr(img, &bpp, &line_len, &endian);
// Direct pixel access: data[y * line_len + x * (bpp / 8)]
```

### `mlx_put_image_to_window(mlx_ptr, win_ptr, img_ptr, x, y)`
**Purpose**: Render image to window at specified position.

**Use Cases**:
- Displaying rendered raytracer output
- Showing texture previews
- Rendering UI elements
- Blitting images to screen

**Example**:
```c
mlx_put_image_to_window(mlx_ptr, win_ptr, img, 0, 0);
```

### `mlx_destroy_image(mlx_ptr, img_ptr)`
**Purpose**: Free image memory.

**Use Cases**:
- Cleaning up loaded textures
- Memory management
- Freeing render targets

**Example**:
```c
mlx_destroy_image(mlx_ptr, img);
```

---

## Color & RGB System

### `mlx_get_color_value(mlx_ptr, color)`
**Purpose**: Convert RGB color to display-specific format.

**Use Cases**:
- Color format conversion
- Ensuring proper color display
- Cross-platform color compatibility

**Example**:
```c
int display_color = mlx_get_color_value(mlx_ptr, 0xFF0000);
```

### `mlx_rgb(red, green, blue)`
**Purpose**: Create RGB color value from components.

**Use Cases**:
- Creating colors from RGB values
- Color manipulation in raytracer
- Dynamic color generation
- Color interpolation

**Example**:
```c
int red_color = mlx_rgb(255, 0, 0);
int custom_color = mlx_rgb(128, 64, 192);
```

### RGB Color Database
**Purpose**: 700+ predefined color names.

**Use Cases**:
- Easy color selection
- UI color theming
- Named color constants
- Color palette management

**Available Colors**:
```c
// Basic colors
red, green, blue, white, black, yellow, cyan, magenta
// Extended colors
forestgreen, skyblue, darkred, lightgray, orange, purple
// And 700+ more...
```

---

## Font & Text System

### `mlx_string_put(mlx_ptr, win_ptr, x, y, color, string)`
**Purpose**: Render text string to window.

**Use Cases**:
- Displaying FPS counter
- Showing raytracer statistics
- UI text labels
- Debug information
- Status messages

**Example**:
```c
mlx_string_put(mlx_ptr, win_ptr, 10, 10, 0xFFFFFF, "FPS: 60");
mlx_string_put(mlx_ptr, win_ptr, 10, 30, 0x00FF00, "Rays: 1000000");
```

### `mlx_set_font(mlx_ptr, win_ptr, font_name)`
**Purpose**: Change the font used for text rendering.

**Use Cases**:
- Customizing UI appearance
- Different fonts for different text types
- Font size adjustment
- Style variations

**Example**:
```c
mlx_set_font(mlx_ptr, win_ptr, "fixed");
```

### Built-in Font Atlas
**Purpose**: Pre-rendered font bitmap for fast text rendering.

**Use Cases**:
- Fast text rendering
- Consistent font appearance
- No external font dependencies
- Embedded font system

---

## Event Handling

### `mlx_loop(mlx_ptr)`
**Purpose**: Start the main event loop.

**Use Cases**:
- Keeping application running
- Processing user input
- Handling window events
- Main application loop

**Example**:
```c
mlx_loop(mlx_ptr); // Blocks until application exits
```

### `mlx_loop_hook(mlx_ptr, function, param)`
**Purpose**: Set function to be called every frame.

**Use Cases**:
- Animation loops
- Continuous rendering
- Real-time updates
- Game loops

**Example**:
```c
int render_frame(void *param)
{
    // Render new frame
    return (0);
}
mlx_loop_hook(mlx_ptr, render_frame, NULL);
```

### `mlx_key_hook(win_ptr, function, param)`
**Purpose**: Handle keyboard input.

**Use Cases**:
- Camera controls (WASD movement)
- Rendering mode switching
- Menu navigation
- Keyboard shortcuts

**Example**:
```c
int key_handler(int keycode, void *param)
{
    if (keycode == 65307) // ESC key
        exit(0);
    return (0);
}
mlx_key_hook(win_ptr, key_handler, NULL);
```

### `mlx_mouse_hook(win_ptr, function, param)`
**Purpose**: Handle mouse button clicks.

**Use Cases**:
- Object selection
- UI button clicks
- Camera rotation
- Menu interactions

**Example**:
```c
int mouse_handler(int button, int x, int y, void *param)
{
    printf("Mouse button %d clicked at (%d, %d)\n", button, x, y);
    return (0);
}
mlx_mouse_hook(win_ptr, mouse_handler, NULL);
```

### `mlx_expose_hook(win_ptr, function, param)`
**Purpose**: Handle window redraw events.

**Use Cases**:
- Redrawing after window resize
- Refreshing display
- Handling window damage
- Maintaining display consistency

**Example**:
```c
int expose_handler(void *param)
{
    // Redraw window contents
    return (0);
}
mlx_expose_hook(win_ptr, expose_handler, NULL);
```

### `mlx_hook(win_ptr, event, mask, function, param)`
**Purpose**: Handle specific X11 events.

**Use Cases**:
- Custom event handling
- Advanced input processing
- Window management events
- System-level events

**Example**:
```c
int close_handler(void *param)
{
    exit(0);
}
mlx_hook(win_ptr, 17, 0, close_handler, NULL); // Window close
```

---

## OpenGL Specific Features

### Shader Support (`mlx_shaders.c`)
**Purpose**: Custom vertex and fragment shaders.

**Use Cases**:
- Hardware-accelerated raytracing
- Custom lighting models
- Post-processing effects
- Advanced rendering techniques

**Example**:
```c
// Load and compile custom shaders
// Hardware-accelerated rendering
```

### OpenGL Integration
**Purpose**: Direct OpenGL context access.

**Use Cases**:
- Advanced 3D rendering
- Hardware acceleration
- Custom OpenGL calls
- Performance optimization

### Hardware Acceleration
**Purpose**: GPU-based rendering.

**Use Cases**:
- Real-time raytracing
- Complex scene rendering
- High-performance graphics
- Smooth animations

---

## Advanced Utilities

### Anti-aliasing Support
**Purpose**: Smooth graphics rendering.

**Use Cases**:
- Reducing jagged edges
- Improving image quality
- Professional rendering
- Smooth curves and lines

### Multiple Window Support
**Purpose**: Creating complex UI layouts.

**Use Cases**:
- Multiple viewport rendering
- Tool windows
- Dialog boxes
- Multi-monitor support

### Custom Cursors
**Purpose**: Interactive elements.

**Use Cases**:
- Context-sensitive cursors
- UI feedback
- Interactive tools
- Custom mouse pointers

### Fullscreen Modes
**Purpose**: Immersive rendering.

**Use Cases**:
- Presentation mode
- Game-like experience
- Distraction-free rendering
- Performance optimization

---

## Practical Examples

### Basic Raytracer Integration
```c
// 1. Initialize MLX
void *mlx = mlx_init();
void *win = mlx_new_window(mlx, 800, 600, "Raytracer");

// 2. Create image buffer
void *img = mlx_new_image(mlx, 800, 600);
char *data = mlx_get_data_addr(img, &bpp, &line_len, &endian);

// 3. Render raytracer output
for (int y = 0; y < 600; y++) {
    for (int x = 0; x < 800; x++) {
        int color = raytrace_pixel(x, y);
        put_pixel_to_image(data, x, y, color, bpp, line_len);
    }
}

// 4. Display result
mlx_put_image_to_window(mlx, win, img, 0, 0);
mlx_loop(mlx);
```

### Texture Loading for 3D Objects
```c
// Load texture
void *texture = mlx_png_file_to_image(mlx, "wood.png", &tex_w, &tex_h);
char *tex_data = mlx_get_data_addr(texture, &bpp, &line_len, &endian);

// Sample texture in raytracer
int sample_texture(float u, float v) {
    int x = (int)(u * tex_w) % tex_w;
    int y = (int)(v * tex_h) % tex_h;
    return get_pixel_from_image(tex_data, x, y, bpp, line_len);
}
```

### Real-time FPS Display
```c
int render_loop(void *param) {
    static int frame_count = 0;
    static time_t last_time = 0;
    
    // Render frame
    raytrace_scene();
    
    // Update FPS
    frame_count++;
    time_t current_time = time(NULL);
    if (current_time > last_time) {
        char fps_str[50];
        sprintf(fps_str, "FPS: %d", frame_count);
        mlx_string_put(mlx, win, 10, 10, 0xFFFFFF, fps_str);
        frame_count = 0;
        last_time = current_time;
    }
    
    return 0;
}
```

---

## Best Practices

### Memory Management
- Always call `mlx_destroy_image()` for loaded images
- Clean up windows with `mlx_destroy_window()`
- Free MLX resources on exit

### Performance Optimization
- Use image buffers for frequent updates
- Minimize `mlx_pixel_put()` calls
- Use hardware acceleration when available
- Cache loaded textures

### Error Handling
- Check return values from MLX functions
- Handle failed image loading gracefully
- Validate input parameters

### Code Organization
- Separate rendering from MLX management
- Use proper event handling
- Modular texture/image management

---

## Common Use Cases by Project Type

### Raytracer Projects
- **`mlx_new_image()`** - Create render target
- **`mlx_get_data_addr()`** - Direct pixel access
- **`mlx_png_file_to_image()`** - Load textures/bump maps
- **`mlx_string_put()`** - Display statistics
- **`mlx_key_hook()`** - Camera controls

### Game Development
- **`mlx_xpm_file_to_image()`** - Load sprites
- **`mlx_loop_hook()`** - Game loop
- **`mlx_mouse_hook()`** - Player input
- **Shader support** - Visual effects

### UI Applications
- **`mlx_string_put()`** - Text display
- **`mlx_mouse_hook()`** - Button clicks
- **Multiple windows** - Complex layouts
- **Color database** - Theming

### Educational Projects
- **`mlx_pixel_put()`** - Learning graphics
- **Basic functions** - Understanding concepts
- **Event handling** - Interactive learning

---

This comprehensive course covers all MLX OpenGL functions with detailed use cases and practical examples. Use this as a reference for implementing advanced graphics features in your projects.
