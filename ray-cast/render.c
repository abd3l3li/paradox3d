/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:54:24 by her-rehy          #+#    #+#             */
/*   Updated: 2025/02/19 18:30:52 by her-rehy         ###   ########.fr       */
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
		put_pixel_to_image(x, y, 0x87CEEB, cube);
		y++;
	}
	while (y < end && y < cube->height)
	{
		put_pixel_to_image(x, y, color, cube);
		y++;
	}
	while (y < cube->height)
	{
		put_pixel_to_image(x, y, 0x808080, cube);
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
			if (v_map->map[y][x] == 'W')
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
	int			mx = (int)(d.ray_x / 70), my;
	float		dx = d.ray_x - base_x, dy;

	find_player_position(cube->cub->v_map, &cube->player_px, &cube->player_py);
	base_x = (cube->player_px * 70) + 35 + cube->x;
	base_y = (cube->player_py * 70) + 35 + cube->y;
	d.ray_x = base_x;
	d.ray_y = base_y;
	d.ray_cos = cos(angle);
	d.ray_sin = sin(angle);
	while (1)
	{
		mx = (int)(d.ray_x / 70), my = (int)(d.ray_y / 70);
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

	pi = PI;
	render.angle_step = (FOV * pi / 180) / NUM_RAYS;
	render.x = 0;
	render.i = 0;
	render.color = 0x4682B4;
	while (render.i < cube->width * cube->height)
	{
		((int *)cube->addr)[render.i] = 0;
		render.i++;
	}
	render.ray_angle = cube->player_angle - (FOV * pi / 180) / 2;
	while (render.x < NUM_RAYS)
	{
		render.distance = distance_to_wall(cube, render.ray_angle);
		render.distance = render.distance * cos(render.ray_angle
				- cube->player_angle);
		render.wall_height = (WALL_HEIGHT / render.distance) * (cube->height
				/ 2);
		draw_vertical_line(cube, render.x, render.wall_height, render.color);
		render.ray_angle += render.angle_step;
		render.x++;
	}
	mlx_put_image_to_window(cube->mlx, cube->win, cube->img, 0, 0);
}
