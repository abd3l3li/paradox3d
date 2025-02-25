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

void	draw_vertical_line(t_cube *cube, int x, float wall_height, int color)
{
	int	start;
	int	end;
	int	y;

	y = 0;
	start = (cube->height - wall_height) / 2;
	end = (cube->height + wall_height) / 2;
	while (y < start)
	{
		put_pixel_to_image(x, y, cube->ceiling_color, cube);//understand this
		y++;
	}
	while (y < end && y < cube->height)
	{
		put_pixel_to_image(x, y, 0x03adfc, cube);//understand this
		y++;
	}
	while (y < cube->height)
	{
		put_pixel_to_image(x, y, cube->floor_color, cube);
		y++;
	}
}

static void	find_player_position(t_map *v_map, float *player_x, float *player_y)
{
	int	y;
	int	x;

	y = 0;
	*player_x = 0;
	*player_y = 0;
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

float	distance_to_wall(t_cube *cube, float angle)
{
	t_distance	d;
	float	base_x;
	float	base_y;
	int			mx = (int)(d.ray_x / TILE_SIZE), my;
	float		dx = d.ray_x - base_x, dy;

	find_player_position(cube->cub->v_map, &cube->player_px, &cube->player_py);
	base_x = (cube->player_px * TILE_SIZE) + 150 + cube->x;
	base_y = (cube->player_py * TILE_SIZE) + 150 + cube->y;
	d.ray_x = base_x;
	d.ray_y = base_y;
	d.ray_cos = cos(angle);
	d.ray_sin = sin(angle);
	while (1)
	{
		mx = (int)(d.ray_x / TILE_SIZE), my = (int)(d.ray_y / TILE_SIZE);
		if (!cube->cub->v_map->map[my] || !cube->cub->v_map->map[my][mx])
			return (d.distance);
		if (cube->cub->v_map->map[my][mx] == '1')
		{
			dx = d.ray_x - base_x, dy = d.ray_y - base_y;
			return (sqrtf(dx * dx + dy * dy));
		}
		d.ray_x += d.ray_cos;
		d.ray_y += d.ray_sin;
		d.distance++;
	}
}

void	cube_render(t_cube *cube)
{
	t_render	render;
	float		pi;
	int i;

	pi = PI;
	render.angle_step = (FOV * pi / 180) / NUM_RAYS;
	i = 0;
	render.color = 0x4682B4;
	render.ray_angle = cube->player_angle - (FOV * pi / 180) / 2;
	while (i < NUM_RAYS)
	{
		render.distance = distance_to_wall(cube, render.ray_angle);
		render.distance = render.distance * cos(render.ray_angle
				- cube->player_angle);
		render.wall_height = (WALL_HEIGHT / render.distance) * (cube->height
				/ 2);
		draw_vertical_line(cube, i, render.wall_height, render.color);
		render.ray_angle += render.angle_step;
		i++;
	}
	mlx_put_image_to_window(cube->mlx, cube->win, cube->img, 0, 0);
}