/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:58:15 by her-rehy          #+#    #+#             */
/*   Updated: 2025/03/05 01:50:12 by her-rehy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

int	is_wall(float x, float y, t_cube *cube)
{
	float	buffer;

	buffer = 0.2;
	cube->player_px += x / TILE_SIZE;
	cube->player_py += y / TILE_SIZE;
	cube->player_py += 0.7;
	cube->player_px += 0.7;
	if (cube->cub->v_map->map[(int)(cube->player_py
			- buffer)][(int)(cube->player_px - buffer)] == '1'
		|| cube->cub->v_map->map[(int)(cube->player_py
			- buffer)][(int)(cube->player_px - buffer)] == ' ')
		return (1);
	return (0);
}


int	key_press(int key, t_cube *cube)
{
	if (key == 65307)
		destroy_cube(cube);
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

static void	handle_movement(t_cube *cube, float *new_x, float *new_y)
{
	const float	pi = PI;

	if (cube->keys.w)
	{
		*new_x = cube->x + cos(cube->player_angle) * MOVE_SPEED;
		*new_y = cube->y + sin(cube->player_angle) * MOVE_SPEED;
	}
	if (cube->keys.s)
	{
		*new_x = cube->x - cos(cube->player_angle) * MOVE_SPEED;
		*new_y = cube->y - sin(cube->player_angle) * MOVE_SPEED;
	}
	if (cube->keys.a)
	{
		*new_x = cube->x + cos(cube->player_angle - pi / 2) * MOVE_SPEED;
		*new_y = cube->y + sin(cube->player_angle - pi / 2) * MOVE_SPEED;
	}
	if (cube->keys.d)
	{
		*new_x = cube->x + cos(cube->player_angle + pi / 2) * MOVE_SPEED;
		*new_y = cube->y + sin(cube->player_angle + pi / 2) * MOVE_SPEED;
	}
}

int	game_loop(t_cube *cube)
{
	float	new_x;
	float	new_y;
	float	pi;

	new_x = cube->x;
	new_y = cube->y;
	if (cube->keys.left)
	{
		cube->player_angle -= ROTATION_SPEED;
		if (cube->player_angle < 0)
			cube->player_angle += 2 * cube->pi;
	}
	if (cube->keys.right)
	{
		cube->player_angle += ROTATION_SPEED;
		if (cube->player_angle > 2 * cube->pi)
			cube->player_angle -= 2 * cube->pi;
	}
	handle_movement(cube, &new_x, &new_y);
	if (!is_wall(new_x, new_y, cube))
	{
		cube->x = new_x;
		cube->y = new_y;
	}
	return (cube_render(cube), 0);
}
// int destroy_window(t_cube *cube)
// {
// 	destroy_cube(0, cube);
// 	exit(0);
// }
void	event_init(t_cube *cube)
{
	
	cube->keys.a = 0;
	cube->keys.d = 0;
	cube->keys.w = 0;
	cube->keys.s = 0;
	cube->keys.left = 0;
	cube->keys.right = 0;
	cube->player_angle = player_facing(cube); //added
	cube->player_angle += ROTATION_SPEED;
	mlx_hook(cube->win, 2, 1L << 0, key_press, cube);
	mlx_hook(cube->win, 3, 1L << 1, key_release, cube);
	mlx_loop_hook(cube->mlx, game_loop, cube);
	mlx_hook(cube->win, 17, 1L << 17, destroy_cube, cube);
}
