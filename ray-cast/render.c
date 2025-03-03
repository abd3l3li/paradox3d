/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:54:24 by her-rehy          #+#    #+#             */
/*   Updated: 2025/03/03 22:42:53 by her-rehy         ###   ########.fr       */
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

float distance_to_wall(t_cube *cube, float angle, float *hit_x, float *hit_y)
{
    t_distance d;

    find_player_position(cube->cub->v_map, &cube->player_px, &cube->player_py);

    float base_x = (cube->player_px * TILE_SIZE) + (TILE_SIZE / 2) + cube->x;
    float base_y = (cube->player_py * TILE_SIZE) + (TILE_SIZE / 2) + cube->y;

    d.ray_x = base_x;
    d.ray_y = base_y;
    d.ray_cos = cos(angle);
    d.ray_sin = sin(angle);
    d.distance = 0;
    d.hit_side = 0;

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

    while (1)
    {
        int mx = (int)(d.ray_x / TILE_SIZE);
        int my = (int)(d.ray_y / TILE_SIZE);

        if (my < 0 || my >= map_height || mx < 0 || mx >= map_width)
            break;

        if (cube->cub->v_map->map[my][mx] == '1' || cube->cub->v_map->map[my][mx] == ' ')
        {
            float dx = d.ray_x - base_x;
            float dy = d.ray_y - base_y;
            *hit_x = d.ray_x;
            *hit_y = d.ray_y;

            if ((int)(d.ray_x / TILE_SIZE) != (int)((d.ray_x - d.ray_cos) / TILE_SIZE))
                cube->hit_side = 0;
            else
                cube->hit_side = 1;

            return sqrtf(dx * dx + dy * dy);
        }

        d.ray_x += d.ray_cos ;
        d.ray_y += d.ray_sin ;
        d.distance ;
    }

    *hit_x = d.ray_x;
    *hit_y = d.ray_y;
    cube->hit_side = 0;
    return INFINITY;
}

void cube_render(t_cube *cube)
{
    t_render render;
    float pi = PI;

    render.angle_step = (FOV * pi / 180.0f) / cube->width;
    render.ray_angle = cube->player_angle - (FOV * pi / 180.0f) / 2;
    render.color = 0x4682B4;

    for (int ray = 0; ray < cube->width; ray++)
    {
        float hit_x, hit_y;

        float raw_distance = distance_to_wall(cube, render.ray_angle, &hit_x, &hit_y);

        float diff_angle = render.ray_angle - cube->player_angle;
        float corrected_dist = raw_distance * cos(diff_angle);

        float wall_height = (WALL_HEIGHT / corrected_dist) * (cube->height / 2.0f);

        draw_vertical_line(cube, ray, wall_height, render.color, hit_x, hit_y);

        render.ray_angle += render.angle_step;
    }

    mlx_put_image_to_window(cube->mlx, cube->win, cube->img, 0, 0);
}