/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube_3d.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 21:06:17 by her-rehy          #+#    #+#             */
/*   Updated: 2025/01/31 21:18:11 by her-rehy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern float	pi;
#define RIGHT_KEY 65363
#define LEFT_KEY 65361
#define W_KEY 119
#define S_KEY 115
#define A_KEY 97
#define D_KEY 100
#define ESC_KEY 65307

extern float	player_angle;
#define ROTATION_SPEED 0.01
#define MOVE_SPEED 1
#define FOV 60
#define NUM_RAYS 800
#define WALL_HEIGHT 200

typedef struct s_data
{
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
}				t_data;

typedef struct s_keys
{
	int			w;
	int			s;
	int			a;
	int			d;
	int			right;
	int			left;
}				t_keys;
typedef struct s_cube
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			width;
	int			height;
	float		x;
	float		y;
	t_keys		keys;
}				t_cube;

typedef struct s_render
{
	float		distance;
	float		angle_step;
	float		wall_height;
	float		ray_angle;
	int			x;
	int			i;
	int			color;
}				t_render;

typedef struct s_distance
{
	float		ray_x;
	float		ray_y;
	float		ray_cos;
	float		ray_sin;
	float		distance;
	float		step;
}				t_distance;

void			draw_cube(t_cube *cube, int pos_x, int pos_y, int color);
void			cube_render(t_cube *cube);
int				close_window(t_cube *cube);
int				shift(int key, t_cube *cube);
void			event_init(t_cube *cube);
void			put_pixel_to_image(int x, int y, int color, t_cube *cube);
