/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:54:24 by her-rehy          #+#    #+#             */
/*   Updated: 2025/02/08 17:17:43 by her-rehy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"
#include <stdlib.h>
#include <string.h>

// int *string_to_array(char **map)
// {
//     int *array;
//     int i;
//     int j;
//     int size;
//     int array_index;

//     i = 0;
//     size = 0;

//     while (map[i])
//     {
//         size += strlen(map[i]);
//         i++;
//     }
//     array = (int *)malloc(sizeof(int) * size);
//     if (!array)
//         return (NULL);
//     i = 0;
//     array_index = 0;

//     while (map[i])
//     {
//         j = 0;
//         while (map[i][j])
//         {
//             if (map[i][j] == '1')
//                 array[array_index] = 1;
//             else
//                 array[array_index] = 0;
//             j++;
//             array_index++;
//         }
//         i++;
//     }

//     return array;
// }

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
float distance_to_wall(t_cube *cube, float angle)
{
    t_distance distance;
    int map_x;
    int map_y;
    int player_x = 0;
    int player_y = 0;
    int found = 0;

    while (cube->cub->v_map->map[player_y] && !found)
    {
        player_x = 0;
        while (cube->cub->v_map->map[player_y][player_x])
        {
            if (cube->cub->v_map->map[player_y][player_x] == 'W')
            {
                found = 1;
                break;
            }
            player_x++;
        }
        if (!found)
            player_y++;
    }

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
