#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600
#define M_PI 3.14159265358979323846

// Simple Earth texture creation
void create_earth_texture(void *mlx, void *img, char *data, int width, int height)
{
    int x, y;
    int color;
    double u, v;
    double noise, land_mask;
    
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            // Convert to UV coordinates
            u = (double)x / width;
            v = (double)y / height;
            
            // Create simple Earth-like pattern
            noise = sin(u * 20.0) * cos(v * 15.0);
            noise += sin(u * 40.0) * cos(v * 30.0) * 0.5;
            noise += sin(u * 80.0) * cos(v * 60.0) * 0.25;
            
            land_mask = (noise > 0.2) ? 1.0 : 0.0;
            
            if (land_mask > 0.5)
            {
                // Land - green/brown
                if (fabs(v - 0.5) < 0.2 && sin(u * 10.0) > 0.3)
                    color = 0xCD853F; // Brown desert
                else if (v < 0.1 || v > 0.9)
                    color = 0xFFFFFF; // White ice caps
                else
                    color = 0x228B22; // Green land
            }
            else
            {
                // Ocean - blue
                color = 0x1E90FF;
            }
            
            // Apply sphere mapping effect (darker at edges)
            double sphere_u = u * 2.0 - 1.0;
            double sphere_v = v * 2.0 - 1.0;
            double dist = sqrt(sphere_u * sphere_u + sphere_v * sphere_v);
            
            if (dist <= 1.0)
            {
                double brightness = sqrt(1.0 - dist * dist);
                int r = ((color >> 16) & 0xFF) * brightness;
                int g = ((color >> 8) & 0xFF) * brightness;
                int b = (color & 0xFF) * brightness;
                color = (r << 16) | (g << 8) | b;
            }
            else
            {
                color = 0x000000; // Black space
            }
            
            *(int *)(data + (y * width + x) * 4) = color;
        }
    }
}

int main(void)
{
    void *mlx;
    void *win;
    void *img;
    char *data;
    int bits_per_pixel;
    int line_length;
    int endian;
    
    mlx = mlx_init();
    if (!mlx)
    {
        printf("Error: MLX initialization failed\n");
        return (1);
    }
    
    win = mlx_new_window(mlx, WIDTH, HEIGHT, "Earth Texture Test");
    if (!win)
    {
        printf("Error: Window creation failed\n");
        return (1);
    }
    
    img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!img)
    {
        printf("Error: Image creation failed\n");
        return (1);
    }
    
    data = mlx_get_data_addr(img, &bits_per_pixel, &line_length, &endian);
    if (!data)
    {
        printf("Error: Getting image data failed\n");
        return (1);
    }
    
    create_earth_texture(mlx, img, data, WIDTH, HEIGHT);
    
    mlx_put_image_to_window(mlx, win, img, 0, 0);
    
    mlx_loop(mlx);
    
    return (0);
}
