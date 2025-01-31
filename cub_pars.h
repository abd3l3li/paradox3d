#ifndef CUB_PARS_H
#define CUB_PARS_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cube_3d.h"
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

int parse_cub(char *file, t_cub *cub);
void    init_cub(t_cub *cub, char **av);
//void    free_cub(t_cub *cub);

void   free_splited(char **splited);
char *ft_skip(t_cub *cub);
int empty_line(char *line);
int extension_check(char *file);

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


#endif