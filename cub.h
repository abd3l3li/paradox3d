/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: her-rehy <her-rehy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 21:06:17 by her-rehy          #+#    #+#             */
/*   Updated: 2025/03/03 03:01:27 by her-rehy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
#	define CUB_H

#include <math.h>
#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "libft/libft.h"
#include "gnl/get_next_line.h"


#define RIGHT_KEY 65363
#define LEFT_KEY 65361
#define W_KEY 119
#define S_KEY 115
#define A_KEY 97
#define D_KEY 100
#define ESC_KEY 65307
#define PI 3.14159265359
#define ROTATION_SPEED 0.03
#define MOVE_SPEED 10
#define FOV 60
// #define NUM_RAYS 600
#define WALL_HEIGHT 250
#define TILE_SIZE 100


typedef struct s_color
{
    int f_num;
    int c_num;
    int f_r;
    int f_g;
    int f_b;
    int c_r;
    int c_g;
    int c_b;
} t_color;

typedef struct s_texture
{
    char *no;
    char *so;
    char *we;
    char *ea;
    int no_num;
    int so_num;
    int we_num;
    int ea_num;
} t_texture;

typedef struct s_map
{
    int     player;
    char    **main_map;
    char    **map;
    int     map_len;
    int     manner;
    char    *argv;
} t_map;

typedef struct s_cub
{
    t_map *v_map;
    t_texture *v_texture;
    t_color *v_color;
    int fd;
    char *line;
    char *str;
    int flag;
} t_cub;

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
	t_keys		keys;
	t_cub		*cub;
	void		*mlx;
	void		*tex_mlx;
	void		*win;
	void		*img;
	void 		*tex_img;
	char		*addr;
	char 		*tex_addr;
	int			bits_per_pixel;
	int 		tex_bits_per_pixel;
	int			line_length;
	int 		tex_line_length;
	int			endian;
	int 		tex_endian;
	int			width;
	int			tex_width;
	int			height;
	int			tex_height;
	float		x;
	float		y;
	float 		x_map;
	float 		y_map;
	int 		map_height;
	int 		map_width;
	float		player_angle;
	float 		player_px;
	float 		player_py;
	float 		pi;
	int			ceiling_color;
	int			floor_color;
    int			hit_side;
}				t_cube;

typedef struct s_render
{
	float		distance;
	float		angle_step;
	float		wall_height;
	float		ray_angle;
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
	int			hit_side; // 0 for vertical (x-side), 1 for horizontal (y-side)
}				t_distance;




int parse_cub(char *file, t_cub *cub);
//void    free_cub(t_cub *cub);
void free_cub_resources(t_cub *cub, t_cube *cube);
void destroy_cube(t_cube *cube);
void   free_splited(char **splited);
int		empty_line(char *line);
void	drain_gnl(int fd);
int		extension_check(char *file);

void    parse_texture_line(t_cub *cub, char **splited, char *type);
int parse_color_line(t_cub *cub, char *line, char *splited);
int parse_texture_color(t_cub *cub);
int holy_checks(t_cub *cub);

int check_textures(t_cub *cub);
int check_texture_counts(t_cub *cub);
int get_rgb(char *line, t_cub *cub);
int check_color(t_cub *cub);
int count_sep(char *line, char c);

int parse_map(t_cub *cub);
int check_elems(t_cub *cub);
int ft_fill_map(t_cub *cub);
int map_alloc(t_cub *cub);
int ft_rowlen(char **map);
int is_map(char *line, t_cub *cub);
int not_flood_fill(t_cub *cub);
void    start_game(t_cube *cube, t_cub *cub);

void			cube_render(t_cube *cube);
int				close_window(t_cube *cube);
void			event_init(t_cube *cube);
void			put_pixel_to_image(int x, int y, int color, t_cube *cube);
// int				*string_to_array(char **map);

int init_texture(t_cube *cube, char *texture_path);
int get_texture_color(t_cube *cube, int tex_x, int tex_y);
void draw_vertical_line(t_cube *cube, int x, float wall_height, int color, float hit_x, float hit_y);


#endif