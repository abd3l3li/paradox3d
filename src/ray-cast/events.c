/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-baz <abel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:58:15 by her-rehy          #+#    #+#             */
/*   Updated: 2025/03/11 00:18:17 by abel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

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
		*new_x = cube->x + cos(cube->player_angle - PI / 2) * MOVE_SPEED;
		*new_y = cube->y + sin(cube->player_angle - PI / 2) * MOVE_SPEED;
	}
	if (cube->keys.d)
	{
		*new_x = cube->x + cos(cube->player_angle + PI / 2) * MOVE_SPEED;
		*new_y = cube->y + sin(cube->player_angle + PI / 2) * MOVE_SPEED;
	}
}

int	game_loop(t_cube *cube)
{
	float	new_x;
	float	new_y;

	new_x = cube->x;
	new_y = cube->y;
	if (cube->keys.left)
	{
		cube->player_angle -= ROTATION_SPEED;
		if (cube->player_angle < 0)
			cube->player_angle += 2 * PI;
	}
	if (cube->keys.right)
	{
		cube->player_angle += ROTATION_SPEED;
		if (cube->player_angle > 2 * PI)
			cube->player_angle -= 2 * PI;
	}
	handle_movement(cube, &new_x, &new_y);
	if (!is_wall(new_x, new_y, cube))
	{
		cube->x = new_x;
		cube->y = new_y;
	}
	return (cube_render(cube), 0);
}

void	event_init(t_cube *cube)
{
	cube->keys.a = 0;
	cube->keys.d = 0;
	cube->keys.w = 0;
	cube->keys.s = 0;
	cube->keys.left = 0;
	cube->keys.right = 0;
	cube->player_angle = player_facing(cube);
	cube->player_angle += ROTATION_SPEED;
	mlx_hook(cube->win, 2, 1L << 0, key_press, cube);
	mlx_hook(cube->win, 3, 1L << 1, key_release, cube);
	mlx_loop_hook(cube->mlx, game_loop, cube);
	mlx_hook(cube->win, 17, 1L << 17, destroy_cube, cube);
}
