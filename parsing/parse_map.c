#include "../cub.h"

int is_map(char *line)
{
    char *tmp;
    static int manner;

    manner = 4;
    tmp = ft_strtrim(line, " ");
    if (manner > 0) // for making sure we skipped the first 4 texture lines
    {
        if (tmp[0] == 'W' || tmp[0] == 'E' || tmp[0] == 'S' || tmp[0] == 'N')
            return (manner--, 0);
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
        if (is_map(line))
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

static int  to_be_continued(t_cub *cub, int row_len, int line_len)
{
    int y;

    y = 0;
    while (y < row_len)
    {
        if (cub->v_map->map[y][0] != '1')
            return (-1);
        line_len = ft_strlen(cub->v_map->map[y]);
        if (cub->v_map->map[y][line_len - 1] != '1')
            return (-1);
        y++;
    }
    return (0);
}

int check_walls(t_cub *cub)
{
    int x;
    int status;
    int line_len;
    int row_len;


    x = 0;
    status = 0;
    line_len = ft_strlen(cub->v_map->map[0]);
    row_len = ft_rowlen(cub->v_map->map);
    while (x < line_len)
    {
        if (cub->v_map->map[0][x] != '1')
            return (-1);
        x++;
    }
    x = 0;
    line_len = ft_strlen(cub->v_map->map[row_len - 1]);
    while (x < line_len)
    {
        if (cub->v_map->map[row_len -1][x] != '1')
            return (-1);
        x++;
    }
    status = to_be_continued(cub, row_len, line_len);
    return (status);
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
                && cub->v_map->map[i][j] != 'E' && cub->v_map->map[i][j] != 'W')
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

int	ft_fill_map(char *str, t_cub *cub)
{
    int i;

	i = 0;
    if (cub->v_map->map_len == -1)
        return (-1);
    cub->v_map->main_map = (char **)malloc(sizeof(char *) * (cub->v_map->map_len + 1));
    if (cub->v_map->main_map == NULL)
        return (-1);
    cub->v_map->map = (char **)malloc(sizeof(char *) * (cub->v_map->map_len + 1));
    if (cub->v_map->map == NULL)
        return (free(cub->v_map->main_map), -1);
    if (str)
    {
        cub->v_map->main_map[i] = ft_strdup(str);
        cub->v_map->map[i] = ft_strtrim(str, "\n");
        free(str);
        i++;
    }
	while (i < cub->v_map->map_len)
	{
        str = get_next_line(cub->fd);
		if (!str)
            return (free(str), 0);
        cub->v_map->main_map[i] = ft_strdup(str);
		cub->v_map->map[i] = ft_strtrim(str, "\n");
		free(str);
		i++;
	}
    cub->v_map->main_map[i] = NULL;
    cub->v_map->map[i] = NULL;
    return (0);
}

int parse_map(char *line, t_cub *cub)
{
    int state;

    state = 0;
    state = ft_fill_map(line, cub);
    if (state != -1)
        state = check_elems(cub);
    if (state != -1)
        state = check_walls(cub);
    return (state);
}
