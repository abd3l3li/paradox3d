/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:54:24 by her-rehy          #+#    #+#             */
/*   Updated: 2025/03/11 09:39:19 by her-rehy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

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

int	check_collision(t_cube *cube, t_distance *d, t_dis *dis, t_render *rend)
{
	if (cube->cub->v_map->map[dis->my][dis->mx] == '1'
		|| cube->cub->v_map->map[dis->my][dis->mx] == ' '
		|| cube->cub->v_map->map[dis->my][dis->mx] == '\0')
	{
		dis->dx = d->ray_x - dis->base_x;
		dis->dy = d->ray_y - dis->base_y;
		rend->hit_x = d->ray_x;
		rend->hit_y = d->ray_y;
		if ((int)(d->ray_x / TILE_SIZE) != (int)((d->ray_x - d->ray_cos)
				/ TILE_SIZE))
			cube->hit_side = 0;
		else
			cube->hit_side = 1;
		return (1);
	}
	return (0);
}

float	cast_ray(t_cube *cube, t_distance *d, t_dis *dis, t_render *rend)
{
	while (1)
	{
		dis->mx = (int)(d->ray_x / TILE_SIZE);
		dis->my = (int)(d->ray_y / TILE_SIZE);
		if (check_collision(cube, d, dis, rend))
			return (sqrtf(dis->dx * dis->dx + dis->dy * dis->dy));
		d->ray_x += d->ray_cos;
		d->ray_y += d->ray_sin;
	}
	rend->hit_x = d->ray_x;
	rend->hit_y = d->ray_y;
	cube->hit_side = 0;
	return (0);
}

float	distance_to_wall(t_cube *cube, t_distance *d, float angle,
		t_render *rend)
{
	static t_dis	dis;

	find_player_position(cube->cub->v_map, &cube->player_px, &cube->player_py);
	dis.base_x = (cube->player_px * TILE_SIZE) + (TILE_SIZE / 2) + cube->x;
	dis.base_y = (cube->player_py * TILE_SIZE) + (TILE_SIZE / 2) + cube->y;
	d->ray_x = dis.base_x;
	d->ray_y = dis.base_y;
	d->ray_cos = cos(angle);
	d->ray_sin = sin(angle);
	return (cast_ray(cube, d, &dis, rend));
}

void	cube_render(t_cube *cube)
{
	t_render	render;
	t_distance	d;
	float		fov_rad;

	render.ray = 0;
	fov_rad = FOV * PI / 180.0f;
	render.angle_step = fov_rad / cube->width;
	render.ray_angle = cube->player_angle - fov_rad / 2;
	while (render.ray < cube->width)
	{
		render.raw_distance = distance_to_wall(cube, &d, render.ray_angle,
				&render);
		render.diff_angle = render.ray_angle - cube->player_angle;
		render.corrected_dist = render.raw_distance * cos(render.diff_angle);
		render.wall_height = (WALL_HEIGHT / render.corrected_dist)
			* (cube->height / 2.0f);
		draw_vertical_line(cube, &d, &render);
		render.ray_angle += render.angle_step;
		render.ray++;
	}
	mlx_put_image_to_window(cube->mlx, cube->win, cube->img, 0, 0);
}
