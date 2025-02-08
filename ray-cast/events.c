/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:58:15 by her-rehy          #+#    #+#             */
/*   Updated: 2025/02/04 11:59:14 by her-rehy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

int	is_wall(float x, float y, t_cube *cube)
{
	int	map_x;
	int	map_y;
	int	*map;
	int the_wall;
	static int map_width;
	static int map_height;
	int grind_x;
	int grind_y;


	// while(cube->cub->v_map->map[map_height])
	// 	map_height++;
	// while(cube->cub->v_map->map[0][map_width])
	// 	map_width++;
	// grind_x = cube->width / map_width;
	// grind_y = cube->height / map_height;
	// printf("grind_x: %d\n grind_y: %d\n", grind_x, grind_y);
	// map = string_to_array(cube->cub->v_map->map);
	// map_x = (int)(x / grind_x);
	// map_y = (int)(y / grind_y);
	// printf("map_x: %d\n map_y: %d\n", map_x, map_y);
	// printf("map_width: %d\n map_height: %d\n", map_width, map_height);
	// if(cube->cub->v_map->map[map_y][map_x] == '1')
	// {
	// 	printf("cube->cub->v_map->map[map_y][map_x]: %c\n", cube->cub->v_map->map[map_y][map_x]);
	// 	free(map);
	// 	return (1);
	// }
	return (0);
}

int	close_window(t_cube *cube)
{
	mlx_destroy_image(cube->mlx, cube->img);
	mlx_destroy_window(cube->mlx, cube->win);
	mlx_destroy_display(cube->mlx);
	free(cube->mlx);
	exit(0);
}

int	key_press(int key, t_cube *cube)
{
	if (key == 65307)
		close_window(cube);
	if (key == W_KEY)
		cube->keys.w = 1;
	if (key == S_KEY)
		cube->keys.s = 1;
	if (key == LEFT_KEY)
		cube->keys.left = 1;
	if (key == RIGHT_KEY)
		cube->keys.right = 1;
	if (key == A_KEY)
		cube->keys.a = 1;
	if (key == D_KEY)
		cube->keys.d = 1;
	return (0);
}

int	key_release(int key, t_cube *cube)
{
	if (key == W_KEY)
		cube->keys.w = 0;
	if (key == S_KEY)
		cube->keys.s = 0;
	if (key == LEFT_KEY)
		cube->keys.left = 0;
	if (key == RIGHT_KEY)
		cube->keys.right = 0;
	if (key == A_KEY)
		cube->keys.a = 0;
	if (key == D_KEY)
		cube->keys.d = 0;
	return (0);
}

int	game_loop(t_cube *cube)
{
	float	new_x;
	float	new_y;
	float	collision_buffer;

	collision_buffer = 10;
	new_x = cube->x;
	new_y = cube->y;
	if (cube->keys.left)
	{
		player_angle -= ROTATION_SPEED;
		if (player_angle < 0)
			player_angle += 2 * pi;
	}
	if (cube->keys.right)
	{
		player_angle += ROTATION_SPEED;
		if (player_angle > 2 * pi)
			player_angle -= 2 * pi;
	}
	if (cube->keys.w)
	{
		new_x = cube->x + cos(player_angle) * MOVE_SPEED;
		new_y = cube->y + sin(player_angle) * MOVE_SPEED;
	}
	if (cube->keys.s)
	{
		new_x = cube->x - cos(player_angle) * MOVE_SPEED;
		new_y = cube->y - sin(player_angle) * MOVE_SPEED;
	}
	if (cube->keys.a)
	{
		new_x = cube->x + cos(player_angle - pi / 2) * MOVE_SPEED;
		new_y = cube->y + sin(player_angle - pi / 2) * MOVE_SPEED;
	}
	if (cube->keys.d)
	{
		new_x = cube->x + cos(player_angle + pi / 2) * MOVE_SPEED;
		new_y = cube->y + sin(player_angle + pi / 2) * MOVE_SPEED;
	}
	if (!is_wall(new_x, new_y, cube) && !is_wall(new_x + collision_buffer, new_y
			+ collision_buffer, cube) && !is_wall(new_x + collision_buffer, new_y
			- collision_buffer, cube) && !is_wall(new_x - collision_buffer, new_y
			+ collision_buffer, cube) && !is_wall(new_x - collision_buffer, new_y
			- collision_buffer, cube))
	{
		cube->x = new_x;
		cube->y = new_y;
	}
	cube_render(cube);
	return (0);
}

void	event_init(t_cube *cube)
{
	cube->keys.a = 0;
	cube->keys.d = 0;
	cube->keys.w = 0;
	cube->keys.s = 0;
	cube->keys.left = 0;
	cube->keys.right = 0;
	mlx_hook(cube->win, 2, 1L << 0, key_press, cube);
	mlx_hook(cube->win, 3, 1L << 1, key_release, cube);
	mlx_loop_hook(cube->mlx, game_loop, cube);
	mlx_hook(cube->win, 17, 1L << 17, close_window, cube);
}
