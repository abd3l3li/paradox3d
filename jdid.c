

void draw_textured_vertical_line(t_cube *cube, int x, float wall_height, t_texture *tex, double wall_x)
{
    int   draw_start;
    int   draw_end;
    int   y;
    int   tex_x;
    int   tex_y;
    double step;
    double tex_pos;
    int   color;

    // Calculate starting and ending y positions for the wall slice.
    draw_start = (cube->height - wall_height) / 2;
    if (draw_start < 0)
        draw_start = 0;
    draw_end = (cube->height + wall_height) / 2;
    if (draw_end > cube->height)
        draw_end = cube->height;

    // Compute texture x-coordinate from the wall hit's fractional position.
    tex_x = (int)(wall_x * tex->width);
    // (Optionally, you could flip tex_x depending on wall orientation.)

    // Calculate how much to step in the texture for each screen pixel.
    step = (double)tex->height / wall_height;
    // Set the initial texture y-coordinate.
    tex_pos = (draw_start - cube->height / 2 + wall_height / 2) * step;

    // Loop through each vertical pixel in the wall slice.
    for (y = draw_start; y < draw_end; y++)
    {
        tex_y = (int)tex_pos;
        tex_pos += step;
        // Get the color from the texture:
        color = *(unsigned int *)(tex->data + (tex_y * tex->line_length + tex_x * (tex->bits_per_pixel / 8)));
        // Draw the pixel on the screen.
        put_pixel_to_image(x, y, color, cube);
    }
}