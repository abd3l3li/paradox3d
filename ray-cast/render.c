/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:54:24 by her-rehy          #+#    #+#             */
/*   Updated: 2025/03/04 03:09:08 by her-rehy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"
#include <stdlib.h>
#include <string.h>

void	put_pixel_to_image(int x, int y, int color, t_cube *cube)
{
	char	*dst;

	if (x >= 0 && x < cube->width && y >= 0 && y < cube->height)
	{
		dst = cube->addr + (y * cube->line_length + x * (cube->bits_per_pixel
					/ 8));
		*(unsigned int *)dst = color;
	}
}

static void	find_player_position(t_map *v_map, float *player_x, float *player_y)
{
	int	y;
	int	x;

	y = 0;
	while (v_map->map[y])
	{
		x = 0;
		while (v_map->map[y][x])
		{
			if (v_map->map[y][x] == 'W' || v_map->map[y][x] == 'N'
				|| v_map->map[y][x] == 'E' || v_map->map[y][x] == 'S')
			{
				*player_x = (float)x;
				*player_y = (float)y;
				return ;
			}
			x++;
		}
		y++;
	}
}

// New function to handle the raycasting logic
float cast_ray(t_cube *cube, t_distance *d, t_dis *dis, float *hit_x, float *hit_y)
{
    while (1)
    {
        dis->mx = (int)(d->ray_x / TILE_SIZE);
        dis->my = (int)(d->ray_y / TILE_SIZE);
        if (dis->my < 0 || dis->my >= dis->map_height || dis->mx < 0 || dis->mx >= dis->map_width)
            break;
        if (cube->cub->v_map->map[dis->my][dis->mx] == '1' || cube->cub->v_map->map[dis->my][dis->mx] == ' ')
        {
            dis->dx = d->ray_x - dis->base_x;
            dis->dy = d->ray_y - dis->base_y;
            *hit_x = d->ray_x;
            *hit_y = d->ray_y;

            if ((int)(d->ray_x / TILE_SIZE) != (int)((d->ray_x - d->ray_cos) / TILE_SIZE))
                cube->hit_side = 0;
            else
                cube->hit_side = 1;

            return sqrtf(dis->dx * dis->dx + dis->dy * dis->dy);
        }
        d->ray_x += d->ray_cos;
        d->ray_y += d->ray_sin;
        d->distance;
    }
    *hit_x = d->ray_x;
    *hit_y = d->ray_y;
    cube->hit_side = 0;
    return 0;
}

// Updated distance_to_wall function
float distance_to_wall(t_cube *cube, t_distance *d, float angle, float *hit_x, float *hit_y)
{
    static t_dis dis;
    static int row;

    find_player_position(cube->cub->v_map, &cube->player_px, &cube->player_py);
    dis.base_x = (cube->player_px * TILE_SIZE) + (TILE_SIZE / 2) + cube->x;
    dis.base_y = (cube->player_py * TILE_SIZE) + (TILE_SIZE / 2) + cube->y;
    d->ray_x = dis.base_x;
    d->ray_y = dis.base_y;
    d->ray_cos = cos(angle);
    d->ray_sin = sin(angle);
    while (cube->cub->v_map->map[dis.map_height])
        dis.map_height++;
    while (row < dis.map_height) // Replace the for loop with a while loop
    {
        dis.row_length = 0;
        while (cube->cub->v_map->map[row][dis.row_length])
            dis.row_length++;
        if (dis.row_length > dis.map_width)
            dis.map_width = dis.row_length;
        row++; // Increment row counter
    }
    return cast_ray(cube, d, &dis, hit_x, hit_y);
}

void	cube_render(t_cube *cube)
{
	t_render	render;
	t_distance d;
	float		pi;
	float		raw_distance;
	float		diff_angle;
	float		corrected_dist;
	float		wall_height;

	pi = PI;
	render.angle_step = (FOV * pi / 180.0f) / cube->width;
	render.ray_angle = cube->player_angle - (FOV * pi / 180.0f) / 2;
	render.color = 0x4682B4;
	int ray = 0;              // Initialize the ray counter
	while (ray < cube->width) // Replace the for loop with a while loop
	{
		float hit_x, hit_y;
		raw_distance = distance_to_wall(cube, &d, render.ray_angle, &hit_x, &hit_y);
		diff_angle = render.ray_angle - cube->player_angle;
		corrected_dist = raw_distance * cos(diff_angle);
		wall_height = (WALL_HEIGHT / corrected_dist) * (cube->height / 2.0f);
		draw_vertical_line(cube, &d, ray, wall_height, render.color, hit_x, hit_y);
		render.ray_angle += render.angle_step;
		ray++; // Increment the ray counter
	}
	mlx_put_image_to_window(cube->mlx, cube->win, cube->img, 0, 0);
}
