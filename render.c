/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:54:24 by her-rehy          #+#    #+#             */
/*   Updated: 2025/01/26 21:07:40 by her-rehy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube_3d.h"

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

float	distance_to_wall(t_cube *cube, float angle)
{
	t_distance	distance;
	int			map[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0,
					0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1};
	int			map_x;
	int			map_y;

	distance.ray_x = cube->x;
	distance.ray_y = cube->y;
	distance.ray_cos = cos(angle);
	distance.ray_sin = sin(angle);
	distance.distance = 0;
	distance.step = 1.0;
	while (1)
	{
		map_x = (int)(distance.ray_x / 70);
		map_y = (int)(distance.ray_y / 70);
		if (map_x < 0 || map_x >= 11 || map_y < 0 || map_y >= 9)
			break ;
		if (map[map_y * 11 + map_x] == 1)
			return (distance.distance);
		distance.ray_x += distance.ray_cos * distance.step;
		distance.ray_y += distance.ray_sin * distance.step;
		distance.distance += distance.step;
	}
	return (distance.distance);
}

void	cube_render(t_cube *cube)
{
	t_render	render;
	int			shadow;

	render.angle_step = (FOV * pi / 180) / NUM_RAYS;
	render.x = 0;
	render.i = 0;
	render.color = 0x4682B4;
	while (render.i < cube->width * cube->height)
	{
		((int *)cube->addr)[render.i] = 0;
		render.i++;
	}
	render.ray_angle = player_angle - (FOV * pi / 180) / 2;
	while (render.x < NUM_RAYS)
	{
		render.distance = distance_to_wall(cube, render.ray_angle);
		render.distance = render.distance * cos(render.ray_angle
				- player_angle);
		render.wall_height = (WALL_HEIGHT / render.distance) * 300;
		draw_vertical_line(cube, render.x, render.wall_height, render.color);
		render.ray_angle += render.angle_step;
		render.x++;
	}
	mlx_put_image_to_window(cube->mlx, cube->win, cube->img, 0, 0);
}
