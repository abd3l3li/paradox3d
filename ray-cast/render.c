/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:54:24 by her-rehy          #+#    #+#             */
/*   Updated: 2025/02/09 16:40:28 by her-rehy         ###   ########.fr       */
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

float distance_to_wall1(t_cube *cube, float angle) // not accurate but moving
{
    t_distance distance;
    int map_x;
    int map_y;

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

        int row_exists = 0;
        int current_row = 0;
        while (cube->cub->v_map->map[current_row] != NULL)
        {
            if (current_row == map_y)
            {
                row_exists = 1;
                break;
            }
            current_row++;
        }
        if (!row_exists)
            break;

        char *row = cube->cub->v_map->map[map_y];
        int col_exists = 0;
        int current_col = 0;
        while (row[current_col] != '\0')
        {
            if (current_col == map_x)
            {
                col_exists = 1;
                break;
            }
            current_col++;
        }
        if (!col_exists)
            break;

        if (cube->cub->v_map->map[map_y][map_x] == '1')
            return (distance.distance);

        distance.ray_x += distance.ray_cos * distance.step;
        distance.ray_y += distance.ray_sin * distance.step;
        distance.distance += distance.step;
    }

    return (distance.distance);
}

float distance_to_wall(t_cube *cube, float angle)//  accurate but not moving
{
    t_distance distance;
    int map_x;
    int map_y;
    float player_x = 0;
    float player_y = 0;
    int found = 0;

    while (cube->cub->v_map->map[(int)player_y] && !found)
    {
        player_x = 0;
        while (cube->cub->v_map->map[(int)player_y][(int)player_x])
        {
            if (cube->cub->v_map->map[(int)player_y][(int)player_x] == 'W')
            {
                found = 1;
                break;
            }
            player_x++;
        }
        if (!found)
            player_y++;
    }
    player_x += cube->x/50;
    player_y += cube->y/50;
    distance.ray_x = player_x * 70 + 60;
    distance.ray_y = player_y * 70 + 60;
    distance.ray_cos = cos(angle);
    distance.ray_sin = sin(angle);
    distance.distance = 0;
    distance.step = 1; 

    while (1)
    {
        map_x = (int)(distance.ray_x / 70);
        map_y = (int)(distance.ray_y / 70);
        
        if (!cube->cub->v_map->map[map_y] || 
            !cube->cub->v_map->map[map_y][map_x])
            break;


        if (cube->cub->v_map->map[map_y][map_x] == '1')
        {

            float dx = distance.ray_x - (player_x * 70 + 60);
            float dy = distance.ray_y - (player_y * 70 + 60);
            return sqrt(dx * dx + dy * dy);
        }

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
