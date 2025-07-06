# Prebuilt Libraries Documentation

## 📦 Available Libraries

This directory contains prebuilt, optimized libraries ready for use without recompilation:

### **Graphics Libraries:**
- **`libmlx42.a`** - Modern OpenGL-based MLX library (recommended)
- **`libglfw3.a`** - GLFW3 library (required for MLX42)
- **`libmlx_linux.a`** - Standard Linux MLX library (fallback)

### **Utility Libraries:**
- **`libft_mandatory.a`** - libft for mandatory raytracer
- **`libft_bonus.a`** - libft for bonus raytracer

### **Headers:**
- **`include/MLX42/MLX42.h`** - MLX42 header file
- **`include/mlx_linux.h`** - Standard MLX header

---

## 🚀 MLX42 (OpenGL) - Recommended

**Best for**: High-performance raytracing, modern features, cross-platform

### Compilation:
```bash
# Basic compilation
gcc main.c -I./prebuilt_libs/include -L./prebuilt_libs -lmlx42 -lglfw3 -ldl -pthread -lm -o raytracer

# With libft
gcc main.c -I./prebuilt_libs/include -L./prebuilt_libs -lmlx42 -lglfw3 -lft_bonus -ldl -pthread -lm -o raytracer
```

### Key Features:
- ✅ **OpenGL hardware acceleration**
- ✅ **PNG image loading support**
- ✅ **Modern, well-documented API**
- ✅ **Cross-platform compatibility**
- ✅ **Built-in font rendering**
- ✅ **Performance optimized**

### Basic Usage:
```c
#include "MLX42/MLX42.h"

int main()
{
    mlx_t* mlx = mlx_init(800, 600, "Raytracer", true);
    if (!mlx)
        return (1);
    
    // Create image for raytracer output
    mlx_image_t* img = mlx_new_image(mlx, 800, 600);
    if (!img)
        return (1);
    
    // Render raytracer output to image
    for (int y = 0; y < 600; y++) {
        for (int x = 0; x < 800; x++) {
            uint32_t color = raytrace_pixel(x, y);
            mlx_put_pixel(img, x, y, color);
        }
    }
    
    // Display image
    mlx_image_to_window(mlx, img, 0, 0);
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return (0);
}
```

---

## 🔧 Standard MLX Linux - Fallback

**Best for**: Compatibility, simple projects, legacy code

### Compilation:
```bash
# Basic compilation
gcc main.c -I./prebuilt_libs/include -L./prebuilt_libs -lmlx_linux -lXext -lX11 -lm -o raytracer

# With libft
gcc main.c -I./prebuilt_libs/include -L./prebuilt_libs -lmlx_linux -lft_bonus -lXext -lX11 -lm -o raytracer
```

### Key Features:
- ✅ **Stable and proven**
- ✅ **Simple API**
- ✅ **Small size**
- ❌ No hardware acceleration
- ❌ Limited image format support

---

## 📚 libft Libraries

### For Mandatory Project:
```bash
gcc ... -L./prebuilt_libs -lft_mandatory ...
```

### For Bonus Project:
```bash
gcc ... -L./prebuilt_libs -lft_bonus ...
```

---

## 🛠️ Updated Makefile Integration

### Option 1: MLX42 (Recommended)
```makefile
# MLX42 OpenGL Version
PREBUILT_DIR = ../prebuilt_libs
MLX_FLAGS = -L$(PREBUILT_DIR) -lmlx42 -lglfw3 -ldl -pthread -lm
INCLUDES = -I$(PREBUILT_DIR)/include

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -L$(PREBUILT_DIR) -lft_bonus -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
```

### Option 2: Standard MLX
```makefile
# Standard MLX Version
PREBUILT_DIR = ../prebuilt_libs
MLX_FLAGS = -L$(PREBUILT_DIR) -lmlx_linux -lXext -lX11 -lm
INCLUDES = -I$(PREBUILT_DIR)/include

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -L$(PREBUILT_DIR) -lft_bonus -o $(NAME)
```

---

## 🎯 Library Comparison

| Feature | MLX42 (OpenGL) | Standard MLX |
|---------|----------------|--------------|
| **Performance** | ⭐⭐⭐⭐⭐ Hardware accelerated | ⭐⭐⭐ Software rendering |
| **Image Loading** | PNG, XPM | XPM only |
| **Documentation** | ⭐⭐⭐⭐⭐ Excellent | ⭐⭐ Basic |
| **Cross-platform** | ✅ Linux, macOS, Windows | ❌ Linux only |
| **Modern API** | ✅ Clean, intuitive | ❌ Legacy style |
| **Size** | 460KB | 107KB |
| **Dependencies** | GLFW3, OpenGL | X11 |

---

## 💡 Performance Tips

### For Raytracing:
1. **Use MLX42** for best performance
2. **Create single image buffer** and update pixels directly
3. **Use `mlx_put_pixel()`** instead of multiple `mlx_pixel_put()` calls
4. **Batch pixel updates** before displaying

### Memory Management:
1. **All libraries are static** - no runtime dependencies
2. **Images are automatically managed** by MLX42
3. **Call `mlx_terminate()`** to clean up

---

## 🚨 Important Notes

### Build Time Optimization:
- **These libraries are prebuilt** - no more waiting for compilation!
- **libft is ready** - instant linking
- **MLX42 includes GLFW3** - all dependencies included

### Usage Guidelines:
1. **Choose MLX42 for new projects**
2. **Use standard MLX only for compatibility**
3. **Include proper header files**
4. **Link libraries in correct order**

### Troubleshooting:
- **Missing symbols**: Check library linking order
- **Compilation errors**: Verify header paths
- **Runtime errors**: Ensure OpenGL drivers are installed

---

## 📊 File Sizes
```
libmlx42.a      - 460KB (OpenGL + features)
libglfw3.a      - 438KB (Window management)
libmlx_linux.a  - 107KB (Basic MLX)
libft_bonus.a   - 72KB  (Extended libft)
libft_mandatory.a - 72KB (Basic libft)
```

**Total MLX42 setup**: ~970KB  
**Total Standard MLX setup**: ~179KB

---

## 🎉 Quick Start

1. **Copy this prebuilt_libs directory** to your project
2. **Update your Makefile** to use prebuilt libraries
3. **Choose MLX42 or standard MLX** based on your needs
4. **Compile and enjoy fast builds!**

No more waiting for library compilation! 🚀
