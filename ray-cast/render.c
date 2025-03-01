/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:54:24 by her-rehy          #+#    #+#             */
/*   Updated: 2025/03/01 00:54:28 by her-rehy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"
#include <stdlib.h>
#include <string.h>
void put_pixel_to_image(int x, int y, int color, t_cube *cube)
{
    char *dst;

    if (x >= 0 && x < cube->width && y >= 0 && y < cube->height)
    {
        dst = cube->addr + (y * cube->line_length + x * (cube->bits_per_pixel / 8));
        *(unsigned int *)dst = color;
    }
}

////////////////////////////////////////////////////////////////////////////////
// draw_vertical_line: draws one vertical strip of the wall (or ceiling/floor)
////////////////////////////////////////////////////////////////////////////////
void draw_vertical_line(t_cube *cube, int x, float wall_height,
                        int color, float hit_x, float hit_y, int hit_side)
{
    // Screen coordinates for where the wall should be drawn
    int start = (cube->height - (int)wall_height) / 2;
    int end   = (cube->height + (int)wall_height) / 2;

    // ----------------------------------------------------------------------
    // Compute tex_x by using the fractional position within the tile.
    // (This ensures correct horizontal mapping of the texture.)
    // ----------------------------------------------------------------------
    float tileCoord;
    if (hit_side == 0)
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

    // Draw from top to bottom of screen
    for (int y = 0; y < cube->height; y++)
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
    }
}

////////////////////////////////////////////////////////////////////////////////
// find_player_position: locates the player in the map
////////////////////////////////////////////////////////////////////////////////
static void find_player_position(t_map *v_map, float *player_x, float *player_y)
{
    int y = 0;
    while (v_map->map[y])
    {
        int x = 0;
        while (v_map->map[y][x])
        {
            if (v_map->map[y][x] == 'W' || v_map->map[y][x] == 'N' ||
                v_map->map[y][x] == 'E' || v_map->map[y][x] == 'S')
            {
                *player_x = (float)x;
                *player_y = (float)y;
                return;
            }
            x++;
        }
        y++;
    }
}

////////////////////////////////////////////////////////////////////////////////
// distance_to_wall: ray-marches until hitting a '1' cell or going OOB
////////////////////////////////////////////////////////////////////////////////
float distance_to_wall(t_cube *cube, float angle,
                       float *hit_x, float *hit_y, int *hit_side)
{
    t_distance d;

    // Where is the player on the map grid?
    find_player_position(cube->cub->v_map, &cube->player_px, &cube->player_py);

    // Convert map coords to pixel coords (+ some offset?)
// Convert map coords to pixel coords
	float base_x = (cube->player_px * TILE_SIZE) + (TILE_SIZE/2) + cube->x;
	float base_y = (cube->player_py * TILE_SIZE) + (TILE_SIZE/2) + cube->y;

    d.ray_x   = base_x;
    d.ray_y   = base_y;
    d.ray_cos = cos(angle);
    d.ray_sin = sin(angle);
    d.distance = 0;
    d.hit_side = 0;

    // Calculate map dimensions
    int map_height = 0, map_width = 0;
    while (cube->cub->v_map->map[map_height])
        map_height++;
    for (int row = 0; row < map_height; row++)
    {
        int row_length = 0;
        while (cube->cub->v_map->map[row][row_length])
            row_length++;
        if (row_length > map_width)
            map_width = row_length;
    }

    // Ray-march until hitting a wall ('1') or going OOB
    while (1)
    {
        int mx = (int)(d.ray_x / TILE_SIZE);
        int my = (int)(d.ray_y / TILE_SIZE);

        // Out of bounds?
        if (my < 0 || my >= map_height || mx < 0 || mx >= map_width)
            break;

        if (cube->cub->v_map->map[my][mx] == '1')
        {
            // We hit a wall
            float dx = d.ray_x - base_x;
            float dy = d.ray_y - base_y;
            *hit_x = d.ray_x;
            *hit_y = d.ray_y;

            // Check if hit is vertical (x-side) or horizontal (y-side)
            if ((int)(d.ray_x / TILE_SIZE) != (int)((d.ray_x - d.ray_cos) / TILE_SIZE))
                *hit_side = 0;
            else
                *hit_side = 1;

            return sqrtf(dx * dx + dy * dy);
        }

        // Keep marching
        d.ray_x += d.ray_cos;
        d.ray_y += d.ray_sin;
        d.distance += 1.0f;
    }

    // If no wall was hit
    *hit_x = d.ray_x;
    *hit_y = d.ray_y;
    *hit_side = 0;
    return INFINITY;
}

////////////////////////////////////////////////////////////////////////////////
// cube_render: casts rays across the screen & draws vertical slices
////////////////////////////////////////////////////////////////////////////////
void cube_render(t_cube *cube)
{
    t_render render;
    float pi = PI;

    // The angle swept by the FOV across the screen
    render.angle_step = (FOV * pi / 180.0f) / cube->width;
    // Start from left-most ray
    render.ray_angle  = cube->player_angle - (FOV * pi / 180.0f) / 2;
    render.color      = 0x4682B4;

    for (int i = 0; i < cube->width; i++)
    {
        float hit_x, hit_y;
        int hit_side;

        // 1) Get the *raw* distance to the wall
        float raw_distance = distance_to_wall(cube, render.ray_angle,
                                              &hit_x, &hit_y, &hit_side);

        // 2) Apply fish-eye correction *only* to the distance used for wall height
        float diff_angle       = render.ray_angle - cube->player_angle;
        float corrected_dist   = raw_distance * cos(diff_angle);

        // 3) Compute how tall the wall should appear on screen
        float wall_height = (WALL_HEIGHT / corrected_dist) * (cube->height / 2.0f);

        // 4) Draw one vertical slice (use the actual hit_x, hit_y for texture coords!)
        draw_vertical_line(cube, i, wall_height, render.color,
                           hit_x, hit_y, hit_side);

        // Move to the next ray
        render.ray_angle += render.angle_step;
    }

    // Display final image
    mlx_put_image_to_window(cube->mlx, cube->win, cube->img, 0, 0);
}