#include "../cub.h"

int init_texture(t_cube *cube, char *texture_path)
{
    // Reuse the existing mlx instance instead of creating a new one
    // if (!cube->mlx)
    //     cube->mlx = mlx_init();

    cube->tex_img = mlx_xpm_file_to_image(cube->mlx, texture_path, &cube->tex_width, &cube->tex_height);
    if (!cube->tex_img)
        free_cub_resources(cube->cub, cube);
    cube->tex_addr = mlx_get_data_addr(cube->tex_img, &cube->tex_bits_per_pixel, &cube->tex_line_length, &cube->tex_endian);
    if (!cube->tex_addr)
        free_cub_resources(cube->cub, cube);

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
void draw_vertical_line(t_cube *cube, int x, float wall_height,
    int color, float hit_x, float hit_y)
{
// Screen coordinates for where the wall should be drawn
int start = (cube->height - (int)wall_height) / 2;
int end   = (cube->height + (int)wall_height) / 2;

// ----------------------------------------------------------------------
// Compute tex_x by using the fractional position within the tile.
// (This ensures correct horizontal mapping of the texture.)
// ----------------------------------------------------------------------
float tileCoord;
if (cube->hit_side == 0)
{
// Vertical side → the ray hit a north/south face
tileCoord = hit_y / (float)TILE_SIZE;
}
else
{
// Horizontal side → the ray hit an east/west face
tileCoord = hit_x / (float)TILE_SIZE;
}
float wall_hit = tileCoord - floor(tileCoord); // fraction in [0..1)
int tex_x = (int)(wall_hit * (float)cube->tex_width);
if (tex_x < 0) tex_x = 0;
if (tex_x >= cube->tex_width) tex_x = cube->tex_width - 1;
// ----------------------------------------------------------------------

// Draw from top to bottom of screen using while loop
int y = 0;
while (y < cube->height)
{
if (y < start)
{
// Ceiling
put_pixel_to_image(x, y, cube->ceiling_color, cube);
}
else if (y < end)
{
// Wall slice: figure out which row of the texture
float tex_y = (float)(y - start) / wall_height * cube->tex_height;
int tex_i = (int)tex_y;
if (tex_i < 0) tex_i = 0;
if (tex_i >= cube->tex_height) tex_i = cube->tex_height - 1;

int tex_color = get_texture_color(cube, tex_x, tex_i);
if (tex_color == -1)
tex_color = 0x03adfc; // Fallback color
put_pixel_to_image(x, y, tex_color, cube);
}
else
{
// Floor
put_pixel_to_image(x, y, cube->floor_color, cube);
}
y++;
}
}