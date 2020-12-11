void	draw_map(t_vars *vars)
{
	int		i;
	int		j;
	int		size;
	t_point	pos;

	i = -1;
	size = 10;
	while (++i < vars->y)
	{
		j = -1;
		while (++j < vars->x[i])
		{
			if (vars->map[i][j] == '1')
				draw_square(&vars->frame, j * size, i * size, size, 0x81000000);
			else if (vars->map[i][j] == '0')
				draw_square(&vars->frame, j * size, i * size, size, 0x81808080);
			else if (vars->map[i][j] == '2')
				draw_square(&vars->frame, j * size, i * size, size, 0x00FF1111);
			else if (ft_strchr("NEWS", vars->map[i][j]))
				draw_square(&vars->frame, j * size, i * size, size, 0x0011FF11);
		}
	}
	pos.x = vars->player.pos.x * size;
	pos.y = vars->player.pos.y * size;
	draw_circle(&vars->frame, pos, size / 4, 0x00E76F51);
}

void	draw_square(t_image *frame, int x, int y, int radius, int color)
{
	int		i;
	int		j;

	i = x;
	while (i <= radius + x)
	{
		j = y;
		while (j <= radius + y)
		{
			put_pixel(frame, i, j, color);
			j++;
		}
		i++;
	}
}

void	draw_rectangle(t_image *frame, int x1, int y1, int x2, int y2,
						int color)
{
	int		i;
	int		j;

	i = ft_min(x1, x2) - 1;
	while (++i <= ft_max(x1, x2))
	{
		j = ft_min(y1, y2) - 1;
		while (++j <= ft_max(y1, y2))
			put_pixel(frame, i, j, color);
	}
}


void	draw_circle(t_image *frame, t_point center, int radius, int color)
{
	int		x;
	int		y;

	x = -radius;
	while (x <= radius)
	{
		y = -radius;
		while (y <= radius)
		{
			if (x * x + y * y <= radius * radius)
				put_pixel(frame, center.x + x, center.y + y, color);
			y++;
		}
		x++;
	}
}
