/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:54:24 by her-rehy          #+#    #+#             */
/*   Updated: 2025/02/25 18:17:57 by her-rehy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"
#include <stdlib.h>
#include <string.h>

void put_pixel_to_image(int x, int y, int color, t_cube *cube) {
    char *dst;

    if (x >= 0 && x < cube->width && y >= 0 && y < cube->height) {
        dst = cube->addr + (y * cube->line_length + x * (cube->bits_per_pixel / 8));
        *(unsigned int *)dst = color;
    }
}

void draw_vertical_line(t_cube *cube, int x, float wall_height, int color, float hit_x, float hit_y, int hit_side) {
    int start = (cube->height - (int)wall_height) / 2;
    int end = (cube->height + (int)wall_height) / 2;
    start = start < 0 ? 0 : start;
    end = end >= cube->height ? cube->height - 1 : end;

    // Calculate texture X coordinate based on hit position and side
    int tex_x;
    if (hit_side == 0) { // Vertical hit (x-side)
        tex_x = (int)(hit_y) % cube->tex_width;
    } else { // Horizontal hit (y-side)
        tex_x = (int)(hit_x) % cube->tex_width;
    }
    tex_x = abs(tex_x); // Ensure positive
    tex_x = tex_x < 0 ? 0 : tex_x;
    tex_x = tex_x >= cube->tex_width ? cube->tex_width - 1 : tex_x;

    for (int y = 0; y < cube->height; y++) {
        if (y < start) {
            put_pixel_to_image(x, y, cube->ceiling_color, cube);
        } else if (y < end) {
            // Calculate texture Y coordinate
            float tex_y = (y - start) / wall_height * cube->tex_height;
            int tex_i = (int)tex_y % cube->tex_height;
            tex_i = tex_i < 0 ? 0 : tex_i;
            tex_i = tex_i >= cube->tex_height ? cube->tex_height - 1 : tex_i;

            int tex_color = get_texture_color(cube, tex_x, tex_i);
            if (tex_color == -1) tex_color = 0x03adfc; // Default color
            put_pixel_to_image(x, y, tex_color, cube);
        } else {
            put_pixel_to_image(x, y, cube->floor_color, cube);
        }
    }
}

static void find_player_position(t_map *v_map, float *player_x, float *player_y) {
    int y = 0;
    int x;

    *player_x = 0;
    *player_y = 0;
    while (v_map->map[y]) {
        x = 0;
        while (v_map->map[y][x]) {
            if (v_map->map[y][x] == 'W' || v_map->map[y][x] == 'N' ||
                v_map->map[y][x] == 'E' || v_map->map[y][x] == 'S') {
                *player_x = (float)x;
                *player_y = (float)y;
                return;
            }
            x++;
        }
        y++;
    }
}

float distance_to_wall(t_cube *cube, float angle, float *hit_x, float *hit_y, int *hit_side) {
    t_distance d;
    find_player_position(cube->cub->v_map, &cube->player_px, &cube->player_py);
    float base_x = (cube->player_px * TILE_SIZE) + 150 + cube->x;
    float base_y = (cube->player_py * TILE_SIZE) + 150 + cube->y;

    d.ray_x = base_x;
    d.ray_y = base_y;
    d.ray_cos = cos(angle);
    d.ray_sin = sin(angle);
    d.distance = 0;
    d.hit_side = 0;

    int mx, my;

    // Calculate map dimensions dynamically
    int map_height = 0, map_width = 0;
    while (cube->cub->v_map->map[map_height]) map_height++;
    for (int y = 0; y < map_height; y++) {
        int row_length = 0;
        while (cube->cub->v_map->map[y][row_length]) row_length++;
        if (row_length > map_width) map_width = row_length;
    }

    while (1) {
        mx = (int)(d.ray_x / TILE_SIZE);
        my = (int)(d.ray_y / TILE_SIZE);

        // Check if the ray is out of bounds
        if (my < 0 || my >= map_height || mx < 0 || mx >= map_width)
            break;

        if (cube->cub->v_map->map[my][mx] == '1') {
            // Determine hit side
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

        // Step the ray
        d.ray_x += d.ray_cos;
        d.ray_y += d.ray_sin;
        d.distance += 1.0;
    }

    *hit_x = d.ray_x;
    *hit_y = d.ray_y;
    *hit_side = 0;
    return INFINITY; // No wall hit
}

void cube_render(t_cube *cube) {
    t_render render;
    float pi = PI;
    render.angle_step = (FOV * pi / 180) / NUM_RAYS;
    int i = 0;
    render.color = 0x4682B4;
    render.ray_angle = cube->player_angle - (FOV * pi / 180) / 2;

    while (i < NUM_RAYS) {
        float hit_x, hit_y;
        int hit_side;
        render.distance = distance_to_wall(cube, render.ray_angle, &hit_x, &hit_y, &hit_side);
        render.distance *= cos(render.ray_angle - cube->player_angle);
        render.wall_height = (WALL_HEIGHT / render.distance) * (cube->height / 2);
        draw_vertical_line(cube, i, render.wall_height, render.color, hit_x, hit_y, hit_side);
        render.ray_angle += render.angle_step;
        i++;
    }
    mlx_put_image_to_window(cube->mlx, cube->win, cube->img, 0, 0);
}