/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:58:15 by her-rehy          #+#    #+#             */
/*   Updated: 2025/02/18 02:20:49 by her-rehy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"
int is_wa1ll(float x, float y, t_cube *cube)
{
    float player_x = cube->x + x / 70;
    float player_y = cube->y + y / 70;
    float buffer = 0.2;  // Small buffer to account for player size

    // Check four points around the player (corners of the hitbox)
    if (cube->cub->v_map->map[(int)(player_y - buffer)][(int)(player_x - buffer)] == '1' ||
        cube->cub->v_map->map[(int)(player_y - buffer)][(int)(player_x + buffer)] == '1' ||
        cube->cub->v_map->map[(int)(player_y + buffer)][(int)(player_x - buffer)] == '1' ||
        cube->cub->v_map->map[(int)(player_y + buffer)][(int)(player_x + buffer)] == '1')
    {
        return (1);  // Collision detected
    }

    return (0);  // No collision
}
int is_wall(float x, float y, t_cube *cube)
{	
	float player_x = 0;
	float player_y = 0;
	int found = 0;
	float buffer = 0.2; 

	
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
     player_x += x / 70;
     player_y += y / 70;
	 player_y += 0.7;
	 player_x += 0.7;
	 
    if (cube->cub->v_map->map[(int)(player_y - buffer)][(int)(player_x - buffer)] == '1')
        return (1);
	
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

	if (!is_wall(new_x, new_y, cube))
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
