int is_wall(t_scene *img, float x, float y)
{
        char    **map;
        int     map_x;
        int     map_y;
        float        offset;

        map = img->map;
        map_y = (int)(y / BLOCK);
        map_x = (int)(x / BLOCK);
        if (!map || map_y < 0 || map_x < 0 || 
                map_y >= img->map_height || map_x >= img->map_width)
                return (1);
        if (map[map_y][map_x] == '1')
                return (1);
        offset = 0.1;
        map_x = (int)((x + offset) / BLOCK);
        if (map_x < img->map_width && map[map_y][map_x] == '1')
                if (x + offset >= map_x * BLOCK && x + offset < (map_x + 1) * BLOCK)
                        return (1);
        map_x = (int)((x - offset) / BLOCK);
        if (map_x >= 0 && map[map_y][map_x] == '1')
                if (x - offset >= map_x * BLOCK && x - offset < (map_x + 1) * BLOCK)
                        return (1);
        map_x = (int)(x / BLOCK);
        map_y = (int)((y - offset) / BLOCK);
        if (map_y >= 0 && map[map_y][map_x] == '1')
                if (y - offset >= map_y * BLOCK && y - offset < (map_y + 1) * BLOCK)
                        return (1);
        map_y = (int)((y + offset) / BLOCK);
        if (map_y < img->map_height && map[map_y][map_x] == '1')
                if (y + offset >= map_y * BLOCK && y + offset < (map_y + 1) * BLOCK)
                        return (1);
        return (0);
}