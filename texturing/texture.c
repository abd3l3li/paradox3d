#include "../cub.h"

float    player_facing(t_cube *data)
{
    size_t    i;
    int    j;

    j = 0;
    while (j < data->cub->v_map->map_len)
    {
        i = 0;
        while (i < ft_strlen(data->cub->v_map->map[j]))
        {
            if (data->cub->v_map->map[j][i] == 'N')
                return (1.5 * PI);
            else if (data->cub->v_map->map[j][i] == 'S')
                return (0.5 * PI);
            else if (data->cub->v_map->map[j][i] == 'E')
                return (0);
            else if (data->cub->v_map->map[j][i] == 'W')
                return (PI);
            i++;
        }
        j++;
    }
    return (0);
}

int load_textures(t_cube *cube)
{
    // Load the north texture
    cube->tex_img[0].img = mlx_xpm_file_to_image(cube->mlx, cube->cub->v_texture->no,
        &cube->tex_img[0].width, &cube->tex_img[0].height);

    // Load the south texture
    cube->tex_img[1].img = mlx_xpm_file_to_image(cube->mlx, cube->cub->v_texture->so,
        &cube->tex_img[1].width, &cube->tex_img[1].height);

    // Load the west texture
    cube->tex_img[2].img = mlx_xpm_file_to_image(cube->mlx, cube->cub->v_texture->we,
        &cube->tex_img[2].width, &cube->tex_img[2].height);

    // Load the east texture
    cube->tex_img[3].img = mlx_xpm_file_to_image(cube->mlx, cube->cub->v_texture->ea,
        &cube->tex_img[3].width, &cube->tex_img[3].height);

    // Ensure all textures were loaded successfully

    if (cube->tex_img[0].img == NULL || cube->tex_img[1].img == NULL ||
        cube->tex_img[2].img == NULL || cube->tex_img[3].img == NULL)
        return 0;

    // Get the address of the texture images

    cube->tex_img[0].addr = mlx_get_data_addr(cube->tex_img[0].img,
        &cube->tex_img[0].bits_per_pixel, &cube->tex_img[0].line_length,
        &cube->tex_img[0].endian);
    
    cube->tex_img[1].addr = mlx_get_data_addr(cube->tex_img[1].img,
        &cube->tex_img[1].bits_per_pixel, &cube->tex_img[1].line_length,
        &cube->tex_img[1].endian);

    cube->tex_img[2].addr = mlx_get_data_addr(cube->tex_img[2].img,
        &cube->tex_img[2].bits_per_pixel, &cube->tex_img[2].line_length,
        &cube->tex_img[2].endian);

    cube->tex_img[3].addr = mlx_get_data_addr(cube->tex_img[3].img,
        &cube->tex_img[3].bits_per_pixel, &cube->tex_img[3].line_length,
        &cube->tex_img[3].endian);

    return 1;
    
}


int get_texture_color(t_cube *cube, int tex_x, int tex_y, int side)
{
    int color;

    color = 0;
    // Ensure the texture image is loaded
    if (cube->tex_img[side].addr == NULL)
        destroy_cube(cube);
    int offset = tex_y * cube->tex_img[side].line_length + tex_x * (cube->tex_img[side].bits_per_pixel / 8);

    // Ensure the offset is within the bounds of the texture image
    if (offset < 0 || offset >= cube->tex_img[side].line_length * cube->tex_img[side].height)
        destroy_cube(cube);

    // Get the color from the texture image
    color = *(int *)(cube->tex_img[side].addr + offset);

    return color;
}

void draw_vertical_line(t_cube *cube, t_distance *d, int ray, float wall_height,
    int color, float hit_x, float hit_y)
{
// Screen coordinates for where the wall should be drawn
int start = (cube->height - (int)wall_height) / 2;
int end   = (cube->height + (int)wall_height) / 2;
float tileCoord;
int side;

// ----------------------------------------------------------------------
// Compute tex_x by using the fractional position within the tile.
// (This ensures correct horizontal mapping of the texture.)
// ----------------------------------------------------------------------
if (cube->hit_side == 1) //v
{
    tileCoord = hit_x / (float)TILE_SIZE;
    if (d->ray_sin <= 0)
        side = 0;//no
    else
        side = 1;//so
}
else //h
{
    tileCoord = hit_y / (float)TILE_SIZE;
    if (d->ray_cos <= 0)
        side = 2;//ea
    else
        side = 3;//we
}
float wall_hit = tileCoord - floor(tileCoord); // fraction in [0..1)
int tex_x = (int)(wall_hit * (float)cube->tex_img[side].width);
if (tex_x < 0)
    tex_x = 0;
if (tex_x >= cube->tex_img[side].width)
    tex_x = cube->tex_img[side].width - 1;
// ----------------------------------------------------------------------

// Draw from top to bottom of screen using while loop
int y = 0;
while (y < cube->height)
{
    if (y < start)
    {
    // Ceiling
        put_pixel_to_image(ray, y, cube->ceiling_color, cube);
    }
    else if (y < end)
    {
// Wall slice: figure out which row of the texture
    float tex_i = (float)(y - start) / wall_height * cube->tex_img[side].height;
    int tex_y = (int)tex_i;
    if (tex_y < 0) tex_y = 0;
    if (tex_y >= cube->tex_img[side].height) tex_y = cube->tex_img[side].height - 1;

    int tex_color = get_texture_color(cube, tex_x, tex_y, side);
    put_pixel_to_image(ray, y, tex_color, cube);
    }
    else
    {
        // Floor
        put_pixel_to_image(ray, y, cube->floor_color, cube);
    }
    y++;
}
}