#include "../cub_pars.h"

int is_map(char *line, t_cub *cub)
{
    char *tmp;

    tmp = ft_strtrim(line, " ");
    if (tmp[0] == '\0')
        return (0);//recheck
    if (cub->v_map->manner > 0) // for making sure we skipped the first 4 texture lines
    {
        if (tmp[0] == 'W' || tmp[0] == 'E' || tmp[0] == 'S' || tmp[0] == 'N')
            return (cub->v_map->manner--, 0);
    }
    if (tmp[0] == '1' || tmp[0] == '0' || tmp[0] == 'W' 
        || tmp[0] == 'E' || tmp[0] == 'S' || tmp[0] == 'N')
        return (1);
    return (0);
}

int map_alloc(t_cub *cub)
{
    int fd;
    char *line;
    int i;

    i = 0;
    fd = open(cub->v_map->argv, O_RDONLY);
    if (fd == -1)
        return (-1);
    while ((line = get_next_line(fd)) != NULL)
    {
        if (is_map(line, cub))
            i++;
        free(line);
    }
    close(fd);
    return (i);
}

int ft_rowlen(char **map)
{
    int i;

    i = 0;
    while (map[i])
        i++;
    return (i);
}

int is_out_of_bounds(t_cub *cub, int i, int j)
{
    if (i == 0 || j == 0 || cub->v_map->map[i + 1] == NULL 
        || cub->v_map->map[i][j + 1] == '\0' || j >= (int)ft_strlen(cub->v_map->map[i - 1])
        || j >= (int)ft_strlen(cub->v_map->map[i + 1]))
        return (-1);
    return (0);
}

int not_flood_fill(t_cub *cub)
{
    int i;
    int j;

    i = 0;
    while (cub->v_map->map[i])
    {
        j = 0;
        while (cub->v_map->map[i][j])
        {
            if (cub->v_map->map[i][j] == '0' || cub->v_map->map[i][j] == 'W' 
                || cub->v_map->map[i][j] == 'E' || cub->v_map->map[i][j] == 'S' 
                || cub->v_map->map[i][j] == 'N')
            {
                if (is_out_of_bounds(cub, i, j) == -1 ||
                    cub->v_map->map[i - 1][j] == ' ' || cub->v_map->map[i + 1][j] == ' ' 
                    || cub->v_map->map[i][j - 1] == ' ' || cub->v_map->map[i][j + 1] == ' ')
                    return (-1);
            }
            j++;
        }
        i++;
    }
    return (0);
}

int check_elems(t_cub *cub)
{
    int i;
    int j;

    i = 0;
    while (cub->v_map->map[i])
    {
        j = 0;
        while (cub->v_map->map[i][j])
        {
            if (cub->v_map->map[i][j] != '1' && cub->v_map->map[i][j] != '0' 
                && cub->v_map->map[i][j] != 'N' && cub->v_map->map[i][j] != 'S' 
                && cub->v_map->map[i][j] != 'E' && cub->v_map->map[i][j] != 'W' 
                && cub->v_map->map[i][j] != ' ')
                return (-1);
            if (cub->v_map->map[i][j] == 'N' || cub->v_map->map[i][j] == 'S' 
                || cub->v_map->map[i][j] == 'E' || cub->v_map->map[i][j] == 'W')
                cub->v_map->player++;
            j++;
        }
        i++;
    }
    if (cub->v_map->player != 1)
        return (-1);
    return (0);
}

int	ft_fill_map(t_cub *cub)
{
    int i;

	i = 0;
    if (cub->v_map->map_len == -1)
        return (-1);
    cub->v_map->map = (char **)malloc(sizeof(char *) * (cub->v_map->map_len + 1));
    if (cub->v_map->map == NULL)
	{
    	return (-1);
	}
	while (i < cub->v_map->map_len)
	{
		if (!cub->line)
            return (0);
		cub->v_map->map[i] = ft_strtrim(cub->line, "\n");
		free(cub->line);
        cub->line = get_next_line(cub->fd);
		i++;
	}
    cub->v_map->map[i] = NULL;
    return (0);
}

int parse_map(t_cub *cub)
{
    int state;

    state = 0;
    state = ft_fill_map(cub);
    if (state != -1)
        state = check_elems(cub);
    if (state != -1)
        state = not_flood_fill(cub);//here
    return (state);
}
