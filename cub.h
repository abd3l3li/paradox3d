#ifndef CUB_H
#define CUB_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "libft/libft.h"
#include "gnl/get_next_line.h"

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
    char    *argv;
} t_map;

typedef struct s_cub
{
    t_map *v_map;
    t_texture *v_texture;
    t_color *v_color;
    int fd;
    char *str;
} t_cub;

int parse_cub(char *file, t_cub *cub);
void    init_cub(t_cub *cub, char **av);
//void    free_cub(t_cub *cub);

int parse_textures(t_cub *cub);
int check_textures(t_cub *cub);
int empty_line(char *line);

int parse_color(char *line, t_cub *cub);
int check_color(t_cub *cub);
int get_rgb(char *line, t_cub *cub);

int parse_map(char *line, t_cub *cub);
int check_elems(t_cub *cub);
int check_walls(t_cub *cub);
int ft_fill_map(char *str, t_cub *cub);
int map_alloc(t_cub *cub);
int ft_rowlen(char **map);
int is_map(char *line);



#endif