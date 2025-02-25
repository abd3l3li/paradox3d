#include "../cub.h"

int init_texture(t_cube *cube, char *texture_path)
{
    cube->tex_mlx = mlx_init();

    cube->tex_img = mlx_xpm_file_to_image(cube->tex_mlx, texture_path, &cube->tex_width, &cube->tex_height);
    if (!cube->tex_img)
    {
        fprintf(stderr, "Error: Failed to load texture image from %s\n", texture_path);
        return -1;
    }
    cube->tex_addr = mlx_get_data_addr(cube->tex_img, &cube->tex_bits_per_pixel, &cube->tex_line_length, &cube->tex_endian);
    if (!cube->tex_addr)
    {
        fprintf(stderr, "Error: Failed to get texture data address from %s\n", texture_path);
        return -1;
    }
    return 0;
}

int get_texture_color(t_cube *cube, int tex_x, int tex_y)
{
    // Ensure the texture image is loaded
    if (cube->tex_addr == NULL)
    {
        fprintf(stderr, "Error: Texture image not loaded.\n");
        return -1;
    }

    // Calculate the offset in the texture image
    int offset = tex_y * cube->tex_line_length + tex_x * (cube->tex_bits_per_pixel / 8);

    // Ensure the offset is within the bounds of the texture image
    if (offset < 0 || offset >= cube->tex_line_length * cube->tex_height)
    {
        fprintf(stderr, "Error: Texture coordinates out of bounds.\n");
        return -1;
    }

    // Get the color from the texture image
    int color = *(int *)(cube->tex_addr + offset);

    return color;
}